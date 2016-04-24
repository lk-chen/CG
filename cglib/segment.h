#pragma once
#include "point.h"

namespace clk {
	/// <summary> 2D segment class </summary>
	class Segment
	{
	private:
		long double _a, _b, _c;
		std::pair<Point, Point> endpoints;

	public:
		/// <summary> Constructor, make sure first element has
		/// higher Y then smaller X </summary>
		Segment(const Point &p, const Point &q);

		/// <summary> Default Constructor </summary>
		Segment() : a(_a), b(_b), c(_c)
			, first(endpoints.first), second(endpoints.second) { };

		/// <summary> Destructor </summary>
		~Segment();

		/// <summary> readonly a, b, and c </summary>
		const long double &a, &b, &c;

		/// <summary> readonly endpoints </summary>
		const Point &first, &second;

		/// <summary> Return if segment is horizontal </summary>
		bool isHorizontal() const;

		/// <summary> Return if segment is vertical </summary>
		bool isVertical() const;

		/// <summary> Equal operator </summary>
		bool operator==(const Segment &that) const;

		/// <summary> Unequal operator </summary>
		bool operator!=(const Segment &that) const;

		/// <summary> Assign operator </summary>
		Segment &operator=(const Segment &that);

		/// <summary> Compare segments by slope </summary>
		/// <remarks> use vector to represent direction, then
		/// (-1, 0) &lt; (-1, 1) &lt; (0, 1) &lt; (1, 1) </remarks>
		bool compareSlope(const Segment &that) const;
	};


	//bool Segment::compareSlope(const Segment &that) const 

}