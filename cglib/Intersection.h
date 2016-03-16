#pragma once
#include "point.h"

namespace clk {
	typedef std::pair<Point, Point> Segment;

	/// <summary> Intersection <summary>
	class DLLEXP Intersection {
	private:
		/// <summary> Detect if a point is on another segment </summary>
		/// <param name="p"> Point </param>
		/// <param name="seg"> Segment </param>
		/// <returns> Returns true if point p lies on segment seg or
		/// overlap with endpoints of seg </returns>
		static bool pointOnSegment(const Point &p, const Segment &seg);

		/// <summary> Detect potential intersection of two line segments </summary>
		/// <param name="seg1"> Line segment 1 </param>
		/// <param name="seg2"> Line segment 2 </param>
		/// <param name="p"> Potential intersection point. If returns
		/// ture, this is intersection point. Else, it is undefined </param>
		/// <returns> Returns true if there is intersection </returns>
		/// <remarks> If there are many intersection points, p can be any of them </remarks>
		static bool segmentIntersect(const Segment& seg1, const Segment& seg2, Point& p);
	public:
		/// <summary> Bentley-Ottmann Sweep algorithm </summary>
		static std::vector<Point> BOSweep(const std::vector<Segment> &segs);
	};
}