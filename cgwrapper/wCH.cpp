#include "wCH.h"
#include <vector>
#include "CH.h"

namespace wrapper {
	std::vector<clk::Point> CH::getPointsFromCoord(
		array<long double>^ X, array<long double>^ Y)
	{
		auto sz = X->Length;
		std::vector<clk::Point> points;

		if (sz != Y->Length) {
			throw "X and Y have different length.";
		}

		for (int i = 0; i < sz; i++)
			points.push_back(clk::Point(X[i], Y[i]));

		return points;
	}
	
	array<size_t> ^CH::incremental(
		array<long double>^ X, array<long double>^ Y) {
		auto points = getPointsFromCoord(X, Y);
		auto r = clk::CH::incremental(points);

		auto res = gcnew array<size_t>(r.size());
		for (size_t i = 0; i < r.size(); i++)
			res[i] = r[i];

		return res;
	}

	array<size_t>^ CH::giftWrapping(
		array<long double>^ X, array<long double>^ Y)
	{
		auto points = getPointsFromCoord(X, Y);
		auto r = clk::CH::giftWrapping(points);

		auto res = gcnew array<size_t>(r.size());
		for (size_t i = 0; i < r.size(); i++)
			res[i] = r[i];

		return res;
	}
}