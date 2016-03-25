#include "Intersection.h"
#include <vector>
#include <set>
#include <queue>
#include <iostream>
#include <string>

using std::vector;
using std::set;
using std::priority_queue;
using std::min;
using std::max;

namespace clk {
	bool Intersection::pointOnSegment(const Point &p, const Segment &seg) {
		if (seg.first == seg.second)
			return p == seg.first;
		else
			return !p.toLeft(seg.first, seg.second) && !p.toLeft(seg.second, seg.first);
	}

	bool Intersection::segmentIntersect(const Segment& seg1, const Segment& seg2, Point& p) {
		if (pointOnSegment(seg1.first, seg2)) {
			p = seg1.first;
			return true;
		}
		if (pointOnSegment(seg1.second, seg2)) {
			p = seg1.second;
			return true;
		}
		if (pointOnSegment(seg2.first, seg1)) {
			p = seg2.first;
			return true;
		}
		if (pointOnSegment(seg2.second, seg1)) {
			p = seg2.second;
			return true;
		}

		if ((seg1.first.toLeft(seg2.first, seg2.second) ^ seg1.second.toLeft(seg2.first, seg2.second))
			&& (seg2.first.toLeft(seg1.first, seg1.second) ^ seg2.second.toLeft(seg1.first, seg1.second))) {
			auto &a = seg2.first - seg1.first;
			auto &b = seg2.second - seg1.first;
			auto &c = seg1.second - seg1.first;
			auto denom = c.Y()*(a.X() - b.X()) - c.X()*(a.Y() - b.Y());
			if (denom == 0)
				return false;
			else
			{
				auto alpha = (c.X()*b.Y() - c.Y()*b.X()) / denom;

				auto x0 = (alpha*a.X() + (1 - alpha)*b.X());
				auto y0 = (alpha*a.Y() + (1 - alpha)*b.Y());
				p = Point(x0, y0) + seg1.first;
				return true;
			}
		}
		else
			return false;
	}

	vector<Point> Intersection::BOSweep(const vector<Segment> &segs) {
		struct Event {
			Point p;
			const Segment *seg1;
			const Segment *seg2;

			Event(const Point& xp, decltype(seg1) xseg1, decltype(seg2) xseg2) :
				p(xp), seg1(xseg1), seg2(xseg2) {};

			Event &operator=(const Event &e) {
				p = e.p;
				seg1 = e.seg1;
				seg2 = e.seg2;

				return *this;
			}

			bool operator<(const Event& that) const {
				if (p.Y() > that.p.Y()) return false;
				else if (p.Y() < that.p.Y()) return true;
				else if (seg1 && seg2) return false;
				else if (that.seg1 && that.seg2) return true;
				else if (seg1) return false;
				else if (that.seg1) return true;
				else if (seg2) return false;
				else throw "Both seg1 and seg2 of Event are NULL.";
			}
		};
		static decltype(segs[0].first.X()) sweepLine;
		struct CompSegPos {
		private:
			auto getIntX(const Segment &seg) {
				if (seg.isHorizontal())
					return seg.first.X();
				else
					return seg.invSlope()
						*(sweepLine - seg.second.Y())
						+ seg.second.X();
			}

		public:
			bool operator()(const Segment *a, const Segment *b) {
				auto aIntX = getIntX(*a);
				auto bIntX = getIntX(*b);
				if (aIntX < bIntX) return true;
				else if (bIntX < aIntX) return false;
				else
				{
					if (a->isHorizontal()) {
						if (b->isHorizontal())
							return a->second.X() < b->second.X();
						else
							return true;
					}
					else {
						if (b->isHorizontal())
							return false;
						else {
							if (a->invSlope() < b->invSlope())
								return true;
							else if (b->invSlope() < a->invSlope())
								return false;
							else if (a->first.X() < b->first.X())
								return true;
							else if (b->first.X() < a->first.X())
								return false;
							else if (a->second.X() < b->second.X())
								return true;
							else if (b->second.X() < a->second.X())
								return false;
							else
								return (a < b);
						}
					}
				}
			}
		};

		set<const Segment*, CompSegPos> SLS;
		priority_queue<Event> EQ;
		vector<Point> intPoints;
		Point intPoint(0, 0);

		for (auto &seg : segs) {
			if (seg.first.Y() > seg.second.Y()) {
				EQ.push(Event(seg.first, &seg, nullptr));
				EQ.push(Event(seg.second, nullptr, &seg));
			}
			else {
				EQ.push(Event(seg.second, &seg, nullptr));
				EQ.push(Event(seg.first, nullptr, &seg));
			}
		}

		while (!EQ.empty())
		{
			auto e = EQ.top();
			EQ.pop();
			sweepLine = e.p.Y();
			
			if (!e.seg2) {
				SLS.insert(e.seg1);
				auto cur = SLS.find(e.seg1);
				
				if (e.seg1->isHorizontal()) {
					cur++;
					while (cur!=SLS.end())
					{
						if (segmentIntersect(*e.seg1, *(*cur), intPoint))
							EQ.push(Event(intPoint, e.seg1, *cur));
						else
							break;
					}
				}
				else
				{
					if (cur != SLS.begin()) {
						cur--;
						if (segmentIntersect(*e.seg1, *(*cur), intPoint))
							EQ.push(Event(intPoint, e.seg1, *cur));
						cur++;
					}
					cur++;
					if (cur != SLS.end()) {
						if (segmentIntersect(*e.seg1, *(*cur), intPoint))
							EQ.push(Event(intPoint, e.seg1, *cur));
					}
				}
			}
			else if (!e.seg1) {
				SLS.erase(e.seg2);
				auto succ = SLS.upper_bound(e.seg2);
				if (succ != SLS.end() && succ != SLS.begin()) {
					auto pre = succ--;
					succ++;
					if (segmentIntersect(*(*pre), *(*succ), intPoint))
						EQ.push(Event(intPoint, *pre, *succ));
				}
			}
			else {
				intPoints.push_back(e.p);
			}
		}

		return intPoints;
	}

	vector<Point> Intersection::BruteForce(const vector<Segment> &segs) {
		vector<Point> intPoints;
		Point p(0, 0);

		for (size_t i = 0; i < segs.size(); i++)
			for (size_t j = 0; j < i; j++)
			{
				if (segmentIntersect(segs[i], segs[j], p))
					intPoints.push_back(p);
			}

		return intPoints;
	}
}