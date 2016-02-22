#pragma once
#include <vector>

#define DLLEXP __declspec(dllexport)

namespace clk {
	/// <summary> Point in 2-D </summary>
	class DLLEXP Point {
	private:
		/// <summary> X and Y coordinates </summary>
		long double _x, _y;

		/// <summary>
		/// Help the toLeft test  </summary>
		/// <param name="p">
		/// Successor of current instance in CCW <param>
		/// <param name="q">
		/// Precursor of current instance in CCW <param>
		/// <returns>
		/// Doubled signed area of triangle defined
		/// by Point p, Point q and current instance
		/// in counterclockwise(CCW) <returns>
		long double area2(const Point &p, const Point &q) const;

		/// <summary>
		/// Help the toLeft test </summary>
		/// <param name="p">
		/// Defines one side of the range </param>
		/// <param name="q">
		/// Defined another side of the range </param>
		/// <returns>
		/// Return whether current instance lies strictly
		/// inside the range restricted by the parallel
		/// lines defined by two given points </returns>
		bool between(const Point &p, const Point &q) const;
	public:
		/// <summary> Explicity constructor </summary>
		Point(long double x, long double y);

		/// <summary> Copy constructor </summary>
		Point(const Point& p);

		/// <summary> Destructor </summary>
		~Point();

		/// <summary> Equal operator </summary>
		/// <returns>
		/// Return true if two points have the same
		/// X and Y coordinates </returns>
		bool operator==(const Point &p) const;

		/// <summary> get X coordinate </summary>
		long double X() const;

		/// <summary> get Y coordinate </summary>
		long double Y() const;

		/// <summary> To Left Test </summary>
		/// <param name="p"> Start of the vector </param>
		/// <param name="q"> End of the vector </param>
		/// <returns>
		/// Return true iff:
		/// 1) current instance lies strictly to the left
		/// of vector (p,q), or
		/// 2) current instance lies on the extension of
		/// vector (p,q), excluding p and q. </returns>
		bool toLeft(const Point &p, const Point &q) const;

		/// <summary> Mirror image of toLeft </summary>
		bool toRight(const Point &p, const Point &q) const;
	};

	class DLLEXP DistinctPoint : public Point {
	public:
		std::vector<size_t> members;
	public:
		DistinctPoint(const Point& p);
	};
}