#include "segment.h"
#include <limits>

using std::pair;
using std::numeric_limits;

namespace clk {
	Segment::Segment(const Point &p, const Point &q)
		: pair(p, q), a(_a), b(_b), c(_c) {
		if (p.x > q.x)
			std::swap(first, second);
		else if (p.x == q.x && p.y > q.y)
			std::swap(first, second);

		if (p.x == q.x) {
			_b = 0;
			_a = 1;
			_c = p.x;
		}
		else if (p.y == q.y) {
			_a = 0;
			_b = 1;
			_c = p.y;
		}
		else {
			_a = p.y - q.y;
			_b = q.x - p.x;
			_c = a*p.x + b*p.y;
		}
	}

	Segment::~Segment() {}

	bool Segment::isHorizontal() const { return a == 0; }

	bool Segment::isVertical() const { return b == 0; }

	long double Segment::slope() const
	{
		if (!isVertical())
			return (second.y - first.y) / (second.x - first.x);
		else
			return numeric_limits<long double>::max();
	}

	long double Segment::invSlope() const
	{
		if (!isHorizontal())
			return (second.x - first.x) / (second.y - first.y);
		else
			return numeric_limits<long double>::max();
	}
}