#include "segment.h"
#include <limits>

using std::pair;
using std::numeric_limits;
using std::swap;

namespace clk {
	Segment::Segment(const Point &p, const Point &q)
		: endpoints(p, q), a(_a), b(_b), c(_c)
		, first(endpoints.first), second(endpoints.second) {
		if (p.y < q.y)
			swap(endpoints.first, endpoints.second);
		else if (p.y == q.y && p.x > q.x)
			swap(endpoints.first, endpoints.second);

		_a = endpoints.first.y - endpoints.second.y;
		_b = endpoints.second.x - endpoints.first.x;
		_c = a*endpoints.first.x + b*endpoints.first.y;
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
		endpoints.first = that.first;
		endpoints.second = that.second;
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