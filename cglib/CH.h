#pragma once
#include <vector>
#include "point.h"

namespace clk {
	class DLLEXP CH {
	public:
		static std::vector<size_t> incremental(std::vector<Point>);
	};
}