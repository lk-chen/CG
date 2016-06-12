#pragma once
#include "point.h"
#include <vector>
#include <tuple>

namespace clk {
	/// <summary> Triangulation </summary>
	class DLLEXP Triangulation {
	public:
		/// <summary> Triangulating monotone polygons </summary>
		/// <param name="vertices"> Polygon vertices, in counter
		/// clock-wise order. Assume polygon is y-monotone,
		/// and vertices are distinct. </param>
		/// <returns> Segments used to triangulating polygon. </returns>
		static std::vector<std::tuple<size_t, size_t>> monotoneTriangulation(const std::vector<Point>& vertices);
	private:

	};
}