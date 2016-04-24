#include "point.h"
#include <sstream>

namespace clk {
	long double Point::area2(const Point& p, const Point& q) const
	{
		return p._x*(q._y - this->_y)
			- q._x*(p._y - this->_y)
			+ this->_x*(p._y - q._y);
	}

	bool Point::between(const Point &p, const Point &q) const
	{
		return ((p._x - this->_x)*(q._x - this->_x)
			+ (p._y - this->_y)*(q._y - this->_y))
			< 0;
	}

	Point::Point(long double x, long double y) :_x(x), _y(y), x(_x), y(_y) { }

	Point::Point(const Point & p) : _x(p._x), _y(p._y), x(_x), y(_y) { }

	Point::~Point() {}

	bool Point::operator==(const Point &p) const
	{
		return (p._x == this->_x)
			&& (p._y == this->_y);
	}

	bool Point::operator!=(const Point &p) const
	{
		return !(operator==(p));
	}

	Point& Point::operator=(const Point &p)
	{
		_x = p._x;
		_y = p._y;
		return *this;
	}

	Point Point::operator-(const Point &p) const
	{
		return Point(_x - p._x, _y - p._y);
	}

	Point Point::operator+(const Point &p) const
	{
		return Point(_x + p._x, _y + p._y);
	}

	bool Point::toLeft(const Point &p, const Point &q) const
	{
		auto area2 = this->area2(p, q);
		if (area2 > 0) return true;
		else if (area2 < 0) return false;
		else return q.between(p, *this);
	}

	bool Point::toRight(const Point &p, const Point &q) const
	{
		auto area2 = this->area2(p, q);
		if (area2 > 0) return false;
		else if (area2 < 0) return true;
		else return q.between(p, *this);
	}

	std::string Point::toString() const
	{
		std::ostringstream sstr;
		sstr << '(' << _x << ',' << _y << ')';

		return sstr.str();
	}

	DistinctPoint::DistinctPoint(const Point & p) :Point(p) { }
}