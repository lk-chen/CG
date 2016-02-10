#include "wCH.h"
#include <vector>
#include "CH.h"

namespace wrapper {
	array<size_t> ^CH::incremental(
		array<long double>^ X, array<long double>^ Y) {
		auto sz = X->Length;

		if (sz != Y->Length) {
			auto res = gcnew array<size_t>(1);
			res[0] = -1;
			return res;
		}

		std::vector<clk::Point> points;
		for (int i = 0; i < sz; i++)
			points.push_back(clk::Point(X[i], Y[i]));
		auto r = clk::CH::incremental(points);

		auto res = gcnew array<size_t>(sz);
		for (int i = 0; i < sz; i++)
			res[i] = r[i];

		return res;
	}
}