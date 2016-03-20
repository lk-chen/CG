#include "Intersection.h"

using std::vector;

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

		auto &a = seg2.first - seg1.first;
		auto &b = seg2.second - seg1.first;
		auto &c = seg1.second - seg1.first;
		auto denom = c.Y()*(a.X() - b.X()) - c.X()*(a.Y() - b.Y());
		if (denom == 0)
			return false;
		else
		{
			auto alpha = (c.X()*b.Y() - c.Y()*b.X()) / denom;
			if (alpha >= 0 && alpha <= 1) {
				auto x0 = (alpha*a.X() + (1 - alpha)*b.X());
				auto y0 = (alpha*a.Y() + (1 - alpha)*b.Y());
				p = Point(x0, y0) + seg1.first;
				return true;
			}
			else
				return false;
		}
	}

	vector<Point> Intersection::BOSweep(const vector<Segment> &segs) {
		std::vector<Point> intPoints;

		return intPoints;
	}

	vector<Point> Intersection::BruteForce(const vector<Segment> &segs) {
		vector<Point> intPoints;

		for (size_t i = 0; i < segs.size(); i++)
			for (size_t j = 0; j < i; j++)
			{
				Point p(0, 0);
				if (segmentIntersect(segs[i], segs[j], p))
					intPoints.push_back(p);
			}

		return intPoints;
	}
}