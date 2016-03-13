#pragma once
#include <vector>
#include "point.h"

namespace clk {
	/// <summary> Convex Hull <summary>
	class DLLEXP CH {
	private:
		/// <summary>
		/// Get tangent points from given x, to existed polygon </summary>
		/// <param name="x"> Index of point to be inserted </param>
		/// <param name="polygon"> Indices of extreme points of current polygon </param>
		/// <param name="points"> All non-repeated points </param>
		/// <returns>
		/// Retern two tangent points s and t </returns>
		static void getTangent(const size_t x, const std::vector<size_t> &polygon,
			const std::vector<DistinctPoint> &points, size_t &s, size_t &t);

		/// <summary>
		/// Group repeated points into non-repeated points </summary>
		/// <param name="points"> Possibly repeated points </param>
		/// <returns> Distinct points </returns>
		static std::vector<DistinctPoint> groupPoints(const std::vector<Point>& points);

		/// <summary>
		/// Transfer non-repeated points to extreme points </summary>
		/// <param name="dPoints"> Distinct points </param>
		/// <param name="polygon"> Extreme point index under distinct point notation </param>
		/// <returns> Extreme points index </returns>
		static std::vector<size_t> deGroupPoints(const std::vector<DistinctPoint>& dPoints, const std::vector<size_t>& polygon);

		/// <summary>
		/// Find the index of the lowest then leftmost point </summary>
		static size_t LTL(const std::vector<DistinctPoint> &points);
	public:
		/// <summary>
		/// Incremental method of constructing convex hull </summary>
		static std::vector<size_t> incremental(const std::vector<Point>& points);

		/// <summary>
		/// Gift Wrapping method of constructing convex hull, aka Javis March </summary>
		static std::vector<size_t> giftWrapping(const std::vector<Point>& points);

		/// <summary>
		/// Graham Scan method of constructing convex hull </summary>
		static std::vector<size_t> grahamScan(const std::vector<Point>& points);
	};
}