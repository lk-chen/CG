#include "Intersection.h"
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <functional>

using std::vector;
using std::set;
using std::tuple;
using std::function;
using std::make_tuple;
using std::swap;
using std::min;
using std::max;
using std::upper_bound;
using std::lower_bound;

namespace clk {
	bool Intersection::pointOnSegment(const Point &p, const Segment &seg) {
		if (seg.first == seg.second)
			return p == seg.first;
		else
			return !p.toLeft(seg.first, seg.second) && !p.toLeft(seg.second, seg.first);
	}

	bool Intersection::segmentIntersect(const Segment& xseg1, const Segment& xseg2, Point& p) {
		const Segment *p1 = &xseg1, *p2 = &xseg2;
		if (p1 < p2)swap(p1, p2);
		auto &seg1 = *p1, &seg2 = *p2;
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
			auto denom = c.y*(a.x - b.x) - c.x*(a.y - b.y);
			if (denom == 0)
				return false;
			else
			{
				auto alpha = (c.x*b.y - c.y*b.x) / denom;

				auto x0 = (alpha*a.x + (1 - alpha)*b.x);
				auto y0 = (alpha*a.y + (1 - alpha)*b.y);
				p = Point(x0, y0) + seg1.first;
				return true;
			}
		}
		else
			return false;
	}

	vector<tuple<Point, size_t, size_t>> Intersection::BruteForce(const vector<Segment> &segs) {
		vector<tuple<Point, size_t, size_t>> intPoints;
		Point p(0, 0);

		for (size_t i = 0; i < segs.size(); i++)
			for (size_t j = 0; j < i; j++)
			{
				if (segmentIntersect(segs[i], segs[j], p))
					intPoints.push_back(make_tuple(p, i, j));
			}

		return intPoints;
	}

	void Intersection::BOSweepClass::EventQueue::push(const Point & xp, const Segment * seg1, const Segment * seg2) {
		if (pq.key_comp()(sweepPoint, xp)) return;
		pq[xp].insert(seg1);
		if (seg2) pq[xp].insert(seg2);
	}

	long double Intersection::BOSweepClass::SegmentPosition::_intX() const {
		if (seg.isHorizontal()) {
			if (sweepPoint.x < seg.first.x) return seg.first.x;
			else if (seg.second.x < sweepPoint.x) return seg.second.x;
			else return sweepPoint.x;
		}
		else
			return (seg.c - seg.b * sweepPoint.y) / seg.a;
	}

	bool Intersection::BOSweepClass::SegmentPosition::operator<(const SegmentPosition & b) const {
		const auto &a = *this;
		if ((a.sweepPoint != b.sweepPoint) || (a.reverse != b.reverse))
			throw "Segments have different sweep condition.";

		Point intPoint;
		if (!segmentIntersect(a.seg, b.seg, intPoint))
			return a._intX() < b._intX();
		else if (!a.seg.compareSlope(b.seg) && !b.seg.compareSlope(a.seg)) {
			if (reverse) return &b.seg < &a.seg;
			else return &a.seg < &b.seg;
		}
		else if (intPoint.y < a.sweepPoint.y)
			return a.seg.compareSlope(b.seg);
		else if (intPoint.y > a.sweepPoint.y)
			return b.seg.compareSlope(a.seg);
		else {
			if (intPoint.x < a.sweepPoint.x)
				return b.seg.compareSlope(a.seg);
			else if (intPoint.x > a.sweepPoint.x)
				return a.seg.compareSlope(b.seg);
			else if (reverse)
				return b.seg.compareSlope(a.seg);
			else
				return a.seg.compareSlope(b.seg);
		}
	}

	vector<tuple<Point, size_t, size_t>> Intersection::BOSweepClass::compute(const vector<Segment>& segs)
	{
		vector<tuple<Point, size_t, size_t>> intPoints;
		if (segs.size() == 0) return intPoints;
		Point sweepPoint;
		bool reverse = false;
		set<SegmentPosition> SLS;
		EventQueue EQ(sweepPoint);
		Point intPoint;

		for (const auto &seg : segs) {
			sweepPoint = seg.first;
			EQ.push(seg.first, &seg);
			EQ.push(seg.second, &seg);
		}

		while (!EQ.empty())
		{
			const auto &event = EQ.top();
			decltype(SLS.begin()) it1, it2;
			vector<const Segment*> upper, lower, contain;
			sweepPoint = event.first;
			auto sl = *event.second.begin();
			auto sr = *event.second.rbegin();
			reverse = false;

			for (auto hline = event.second.begin(); hline != event.second.end(); ++hline) {
				if ((*hline)->isHorizontal()) {
					for (auto it = SLS.upper_bound(SegmentPosition(**hline, sweepPoint, reverse));
						it != SLS.end(); ++it) {
						if (segmentIntersect(**hline, it->seg, intPoint))
							EQ.push(intPoint, *hline, &it->seg);
						else break;
					}
				}
			}

			for (auto pseg : event.second) {
				SLS.erase(SegmentPosition(*pseg, sweepPoint, reverse));
				if (pseg->first == sweepPoint) lower.push_back(pseg);
				else if (pseg->second == sweepPoint) upper.push_back(pseg);
				else contain.push_back(pseg);
			}

			reverse = true;
			for (auto pseg : lower) SLS.insert(SegmentPosition(*pseg, sweepPoint, reverse));
			for (auto pseg : contain) SLS.insert(SegmentPosition(*pseg, sweepPoint, reverse));

			it2 = SLS.lower_bound(SegmentPosition(*sr, sweepPoint, reverse));
			if (it2 != SLS.begin() && it2 != SLS.end()) {
				it1 = it2; --it1;
				for (auto r = it2; r != SLS.end(); ++r) {
					if (r->seg.compareSlope(it2->seg) || it2->seg.compareSlope(r->seg)) break;
					for (auto l = it1; l != SLS.end(); --l) {
						if (l->seg.compareSlope(it1->seg) || it1->seg.compareSlope(l->seg)) break;
						if (segmentIntersect(l->seg, r->seg, intPoint))
							EQ.push(intPoint, &l->seg, &r->seg);
					}
				}
			}

			it2 = SLS.upper_bound(SegmentPosition(*sl, sweepPoint, reverse));
			if (it2 != SLS.begin() && it2 != SLS.end()) {
				it1 = it2; --it1;
				for (auto r = it2; r != SLS.end(); ++r) {
					if (r->seg.compareSlope(it2->seg) || it2->seg.compareSlope(r->seg)) break;
					for (auto l = it1; l != SLS.end(); --l) {
						if (l->seg.compareSlope(it1->seg) || it1->seg.compareSlope(l->seg)) break;
						if (segmentIntersect(l->seg, r->seg, intPoint))
							EQ.push(intPoint, &l->seg, &r->seg);
					}
				}
			}

			for (auto it1 = event.second.begin(); it1 != event.second.end(); ++it1)
				for (auto it2 = it1; ; ) {
					++it2;
					if (it2 == event.second.end()) break;

					segmentIntersect(**it1, **it2, intPoint);
					if (intPoint == event.first)
						intPoints.push_back(make_tuple(event.first,
							*it1 - &segs[0], *it2 - &segs[0]));
				}

			EQ.pop();
		}

		return intPoints;
	}
}