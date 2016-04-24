#include "segment.h"
#include <limits>

using std::pair;
using std::numeric_limits;

namespace clk {
	Segment::Segment(const Point &p, const Point &q)
		: pair(p, q), a(_a), b(_b), c(_c) {
		if (p.y < q.y)
			std::swap(first, second);
		else if (p.y == q.y && p.x > q.x)
			std::swap(first, second);

		_a = first.y - second.y;
		_b = second.x - first.x;
		_c = a*first.x + b*first.y;
	}

	Segment::~Segment() {}

	bool Segment::isHorizontal() const { return a == 0; }

	bool Segment::isVertical() const { return b == 0; }

	bool Segment::operator==(const Segment & that) const
	{
		return (first == that.first) && (second == that.second);
	}

	bool Segment::operator!=(const Segment & that) const
	{
		return !operator==(that);
	}

	Segment & Segment::operator=(const Segment & that) {
		first = that.first;
		second = that.second;
		_a = that.a;
		_b = that.b;
		_c = that.c;

		return *this;
	}

	bool Segment::compareSlope(const Segment & that) const {
		if (that.a == 0) return false;
		else return b*that.a > a*that.b;
	};
}