#include "wIntersection.h"
#include "Intersection.h"

using std::vector;

namespace wrapper {
	vector<clk::Segment> Intersection::getSegsFromCoord(
		array<long double>^ X, array<long double>^ Y)
	{
		auto sz = X->Length;
		vector<clk::Segment> segs;

		if (sz != Y->Length)
			throw "X and Y have different length.";
		if (sz % 2 != 0)
			throw "Size of X is not even number.";

		for (int i = 0; i < sz / 2; i++)
		{
			clk::Point p(X[2 * i], Y[2 * i]);
			clk::Point q(X[2 * i + 1], Y[2 * i + 1]);
			segs.push_back(clk::Segment(p, q));
		}

		return segs;
	}

	array<long double> ^Intersection::getIntersection(
		array<long double>^ X, array<long double>^ Y)
	{
		auto segs = getSegsFromCoord(X, Y);
		auto intPoints = clk::Intersection::BOSweep(segs);

		auto res = gcnew array<long double>(intPoints.size() * 2);
		for (size_t i = 0; i < intPoints.size(); i++)
		{
			res[2 * i] = intPoints[i].X();
			res[2 * i + 1] = intPoints[i].Y();
		}

		return res;
	}
}