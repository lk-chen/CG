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

	bool Segment::compareSlope(const Segment &that) const {
		bool flip = (a*that.a != 0) ? (a*that.a < 0) : (a + that.a < 0);
		bool result = b*that.a > a*that.b;
		return flip^result;
	}

	bool Segment::parallel(const Segment & that) const
	{
		return compareSlope(that) == that.compareSlope(*this);
	}
}