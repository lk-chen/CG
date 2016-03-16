#include "Intersection.h"

namespace clk {
	bool Intersection::pointOnSegment(const Point &p, const Segment &seg) {
		if (seg.first == seg.second)
			return p == seg.first;
		else
			return !p.toLeft(seg.first, seg.second) && !p.toLeft(seg.second, seg.first);
	}

	bool Intersection::segmentIntersect(const Segment& seg1, const Segment& seg2, Point& p) {
		auto &p1 = seg1.first;
		auto &q1 = seg1.second;
		auto &p2 = seg2.first;
		auto &q2 = seg2.second;

		if (pointOnSegment(p1, seg2)) {
			p = p1;
			return true;
		}
		if (pointOnSegment(q1, seg2)) {
			p = q1;
			return true;
		}
		if (pointOnSegment(p2, seg1)) {
			p = p2;
			return true;
		}
		if (pointOnSegment(q2, seg1)) {
			p = q2;
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
				auto x0 = (alpha*a.X() + (1 - alpha)*b.X())*c.Y();
				auto y0 = (alpha*a.Y() + (1 - alpha)*b.Y())*c.X();
				p = Point(x0, y0);
				return true;
			}
			else
				return false;
		}
	}

	std::vector<Point> Intersection::BOSweep(const std::vector<Segment> &segs) {
		std::vector<Point> intPoints;

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