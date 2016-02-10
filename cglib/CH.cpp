#include "CH.h"

namespace clk {
	std::vector<size_t> CH::incremental(std::vector<Point> points) {
		int n = points.size(), i = 0;

		std::vector<size_t> idx(n);
		for (auto &e : idx)
			e = i++;

		return idx;
	}
}