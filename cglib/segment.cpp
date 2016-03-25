#include "segment.h"
#include <limits>

using std::pair;
using std::numeric_limits;

namespace clk {
	Segment::Segment(const Point &p, const Point &q)
		: pair(p, q) {
		if (p.X() > q.X())
			std::swap(first, second);
		else if (p.X() == q.X() && p.Y() > q.Y())
			std::swap(first, second);
	}

	Segment::~Segment() {}

	bool Segment::isHorizontal() const
	{
		return first.Y() == second.Y();
	}

	bool Segment::isVertical() const
	{
		return first.X() == second.X();
	}

	auto Segment::slope() const
	{
		if (!isVertical())
			return (second.Y() - first.Y()) / (second.X() - first.X());
		else
			return numeric_limits<decltype(first.X())>::max();
	}

	auto Segment::invSlope() const
	{
		if (!isHorizontal())
			return (second.X() - first.X()) / (second.Y() - first.Y());
		else
			return numeric_limits<decltype(first.X())>::max();
	}
}