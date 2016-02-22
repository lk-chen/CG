#pragma once
#include <vector>
#include "point.h"

namespace clk {
	class DLLEXP CH {
	private:
		static void getTangent(const size_t x, const std::vector<size_t> &polygon,
			const std::vector<DistinctPoint> &points, size_t &s, size_t &t);

		static std::vector<DistinctPoint> groupPoints(const std::vector<Point>& points);
	public:
		static std::vector<size_t> incremental(const std::vector<Point>&);
	};
}