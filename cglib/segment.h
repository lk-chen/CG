#pragma once
#include "point.h"

namespace clk {
	/// <summary> 2D segment class </summary>
	class Segment : public std::pair<Point, Point>
	{
	public:
		/// <summary> Constructor, make sure first element has
		/// smaller X then smaller Y </summary>
		Segment(const Point &p, const Point &q);

		/// <summary> Destructor </summary>
		~Segment();

		/// <summary> Return if segment is horizontal </summary>
		bool isHorizontal() const;

		/// <summary> Return if segment is vertical </summary>
		bool isVertical() const;

		/// <summary> Get slope of segment, deltaY / deltaX </summary>
		long double slope() const;

		/// <summary> Get inverse slope of segment, deltaX / deltaY </summary>
		long double invSlope() const;
	private:
	};
}