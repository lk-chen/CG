#include "segment.h"
#include <limits>

using std::pair;
using std::numeric_limits;

namespace clk {
	Segment::Segment(const Point &p, const Point &q)
		: pair(p, q), a(_a), b(_b), c(_c) {
		if (p.X() > q.X())
			std::swap(first, second);
		else if (p.X() == q.X() && p.Y() > q.Y())
			std::swap(first, second);

		if (p.X() == q.X()) {
			_b = 0;
			_a = 1;
			_c = p.X();
		}
		else if (p.Y() == q.Y()) {
			_a = 0;
			_b = 1;
			_c = p.Y();
		}
		else {
			_a = p.Y() - q.Y();
			_b = q.X() - p.X();
			_c = a*p.X() + b*p.Y();
		}
	}

	Segment::~Segment() {}

	bool Segment::isHorizontal() const { return a == 0; }

	bool Segment::isVertical() const { return b == 0; }

	long double Segment::slope() const
	{
		if (!isVertical())
			return (second.Y() - first.Y()) / (second.X() - first.X());
		else
			return numeric_limits<decltype(first.X())>::max();
	}

	long double Segment::invSlope() const
	{
		if (!isHorizontal())
			return (second.X() - first.X()) / (second.Y() - first.Y());
		else
			return numeric_limits<decltype(first.X())>::max();
	}
}