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

	vector<tuple<Point, size_t, size_t>> Intersection::BOSweep::compute(const vector<Segment> &segs, const CallbackType &callback) {
		vector<tuple<Point, size_t, size_t>> intPoints;
		if (segs.size() < 2) return intPoints;
		Point sweepPoint;
		Segment sweepSlope(segs[0].first, segs[1].second);
		SweepLineStatus SLS;
		EventQueue EQ(sweepPoint,sweepSlope);
		Point intPoint;

		for (auto &seg : segs) {
			EQ.push(Event(seg.first, &seg, nullptr));
			EQ.push(Event(seg.second, nullptr, &seg));
		}

		while (!EQ.empty())
		{
			auto event = EQ.top();
			decltype(SLS.begin()) it1, it2, it3, it4;
			sweepPoint = event.p;
			callback();

			if (event.type == Event::start) {
				sweepSlope = *event.seg1;
				SegmentPosition sp(*event.seg1, sweepPoint, sweepSlope);
				SLS.insert(sp);
				it1 = it2 = it3 = it4 = SLS.find(sp);
				it1--;
				it4++;
			}
			else if (event.type == Event::end) {
				sweepSlope = *event.seg2;
				SegmentPosition sp(*event.seg2, sweepPoint, sweepSlope);
				SLS.erase(sp);
				it1 = it2 = it3 = it4 = SLS.upper_bound(sp);
				if (!SLS.empty()) {
					it1--;
					it3--;
				}
			}
			else {
				auto i = event.seg1 - &segs[0];
				auto j = event.seg2 - &segs[0];
				intPoints.push_back(make_tuple(event.p, i, j));
				sweepSlope = event.seg1->compareSlope(*event.seg2)
					? *event.seg1 : *event.seg2;
				SegmentPosition sp1(*event.seg1, sweepPoint, sweepSlope);
				SegmentPosition sp2(*event.seg2, sweepPoint, sweepSlope);
				SLS.erase(sp1);
				SLS.erase(sp2);
				SegmentPosition sp(event.seg1->compareSlope(*event.seg2)
					? *event.seg2 : *event.seg1, sweepPoint, sweepSlope);
				sweepSlope = sp.seg;
				SLS.insert(sp1);
				SLS.insert(sp2);
				it1 = it2 = it3 = it4 = SLS.find(sp);
				it1--;it1--;
				it2--;
				it4++;
			}

			if (it1 != SLS.end()
				&& it2 != SLS.begin() && it2 != SLS.end()
				&& segmentIntersect(it1->seg, it2->seg, intPoint))
				EQ.push(Event(intPoint, &it1->seg, &it2->seg));
			if (it3 != SLS.end()
				&& it4 != SLS.begin() && it4 != SLS.end()
				&& segmentIntersect(it3->seg, it4->seg, intPoint))
				EQ.push(Event(intPoint, &it3->seg, &it4->seg));

			EQ.pop();
		}
		
		return intPoints;
	}

	vector<tuple<Point, size_t, size_t>> Intersection::BOSweep(const std::vector<Segment>& segs, const CallbackType &callback)
	{
		return BOSweep::compute(segs, callback);
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
	
	Intersection::BOSweep::Event::Event(const Point & xp, decltype(seg1) xseg1, decltype(seg2) xseg2) :
		p(xp), seg1(xseg1), seg2(xseg2) {
		if (seg1 && seg2) {
			type = intsect;
			if (seg1 > seg2) swap(seg1, seg2);
		}
		else if (seg1)
			type = start;
		else if (seg2)
			type = end;
		else
			throw "Both seg1 and seg2 of Event are NULL.";
	}

	Intersection::BOSweep::Event & Intersection::BOSweep::Event::operator=(const Event & e) {
		p = e.p;
		seg1 = e.seg1;
		seg2 = e.seg2;
		type = e.type;

		return *this;
	}
	
	bool Intersection::BOSweep::Event::operator<(const Event & that) const {
		if (p.y != that.p.y) return p.y < that.p.y;
		else if (type != that.type) return type < that.type;
		else if (p.x != that.p.x) return p.x > that.p.x;
		else if (seg1 != that.seg1) return seg1 < that.seg1;
		else return seg2 < that.seg2;
	}

	void Intersection::BOSweep::EventQueue::push(Event & val) {
		if (val.type == Event::intsect) {
			if (val.p.y > sweepPoint.y)
				return;
			else if (val.p.y == sweepPoint.y && val.p.x < sweepPoint.x)
				return;
			else if (val.p == sweepPoint
				&& val.seg1->compareSlope(sweepSlope)
				&& val.seg2->compareSlope(sweepSlope))
				return;
		}

		pq.insert(val);
	}

	long double Intersection::BOSweep::CompSegPos::_intX(const SegmentPosition &sp) {
		auto &seg = sp.seg;
		if (seg.isHorizontal()) {
			if (sp.sweepPoint.x < seg.first.x) return seg.first.x;
			else if (seg.second.x < sp.sweepPoint.x) return seg.second.x;
			else return sp.sweepPoint.x;
		}
		else
			return (seg.c - seg.b * sp.sweepPoint.y) / seg.a;
	}
	
	bool Intersection::BOSweep::CompSegPos::operator()(const SegmentPosition &a, const SegmentPosition &b) {
		if ((a.sweepPoint != b.sweepPoint) || (a.sweepSlope != b.sweepSlope))
			throw "Segments have different sweep condition.";

		Point intPoint;
		if (!segmentIntersect(a.seg, b.seg, intPoint)) {
			return _intX(a) < _intX(b);
		}
		else if (a.seg.parallel(b.seg))
			return &a.seg < &b.seg;
		else if (intPoint.y != a.sweepPoint.y) {
			return a.seg.compareSlope(b.seg) ^ (intPoint.y > a.sweepPoint.y);
		}
		else {
			bool result = a.seg.compareSlope(b.seg);
			if (intPoint.x != a.sweepPoint.x)
				return (intPoint.x < a.sweepPoint.x) ^ result;
			else if (a.sweepSlope.compareSlope(a.seg) == b.sweepSlope.compareSlope(b.seg))
				return (!a.sweepSlope.compareSlope(a.seg)) ^ result;
			else
				return result;
		}
	}
}