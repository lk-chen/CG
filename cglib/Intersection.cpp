#include "Intersection.h"
#include <vector>
#include <set>
#include <queue>
#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>

using std::vector;
using std::set;
using std::priority_queue;
using std::tuple;
using std::make_tuple;
using std::swap;
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

	vector<tuple<Point, size_t, size_t>> Intersection::BOSweep::compute(const vector<Segment> &segs) {
		vector<tuple<Point, size_t, size_t>> intPoints;
		SweepLineStatus SLS;
		EventQueue EQ;
		Point sweepLine, intPoint;
		SLS.setSweepLine(&sweepLine);
		EQ.setSweepLine(&sweepLine);
		long double leftBound = 0;

		for (auto &seg : segs) {
			leftBound = min(leftBound, seg.first.X()) - 1;
			if (seg.first.Y() < seg.second.Y()) {
				EQ.push(Event(seg.second, &seg, nullptr));
				EQ.push(Event(seg.first, nullptr, &seg));
			}
			else {
				EQ.push(Event(seg.first, &seg, nullptr));
				EQ.push(Event(seg.second, nullptr, &seg));
			}
		}

		while (!EQ.empty())
		{
			auto event = EQ.top();
			EQ.pop();

			if (event.type == Event::start) {
				auto res = SLS.insert(event.seg1);
				auto it = res.first;
			}
			else if (event.type == Event::end) {
				SLS.erase(event.seg2);
			}
			else {
				intPoints.push_back(make_tuple(event.p, EQ.size(), EQ.size()));
				SLS.erase(event.seg1);
				SLS.insert(event.seg1);
			}
		}
		
		return intPoints;
	}

	vector<tuple<Point, size_t, size_t>> Intersection::BOSweep(const std::vector<Segment>& segs)
	{
		return BOSweep::compute(segs);
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

		return *this;
	}
	
	bool Intersection::BOSweep::Event::operator<(const Event & that) const {
		if (p.Y() != that.p.Y()) return p.Y() < that.p.Y();
		else if (p.X() != that.p.X()) return p.X() > that.p.X();
		else if (type != that.type) return type < that.type;
		else if (seg1 != that.seg1) return seg1 < that.seg1;
		else if (seg2 != that.seg2) return seg2 < that.seg2;
		else throw DupEventError();
	}

	void Intersection::BOSweep::EventQueue::push(value_type & val) {
		if (val.type == Event::intsect) {
			if (val.p.Y() > sweepLine->Y())
				return;
			else if (val.p.Y() == sweepLine->Y() && val.p.X() < sweepLine->X())
				return;
		}
		try
		{
			priority_queue::push(val);
		}
		catch (const DupEventError&)
		{

		}
	}
	
	long double Intersection::BOSweep::CompSegPos::getIntX(const Segment & seg) {
		if (seg.isHorizontal())
			return seg.first.X();
		else
			return (seg.c - seg.b * sweepLine->Y()) / seg.a;
	}
	
	bool Intersection::BOSweep::CompSegPos::operator()(const Segment * a, const Segment * b) {
		auto aIntX = getIntX(*a);
		auto bIntX = getIntX(*b);
		if (aIntX != bIntX) return aIntX < bIntX;
		else return a < b;
		// TODO: what if two segs intersect at the same point.
	}
}