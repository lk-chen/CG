#pragma once
#include "point.h"
#include "segment.h"
#include <exception>
#include <queue>
#include <set>
#include <vector>
#include <tuple>

namespace clk {
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
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		/// <returns> Returns true if there is intersection </returns>
		/// <remarks> If there are many intersection points, p can be any of them </remarks>
		static bool segmentIntersect(const Segment& seg1, const Segment& seg2, Point& p);

		class BOSweep {
		private:
			class DupEventError : public std::exception {};

			struct Event {
				Point p;
				const Segment *seg1;
				const Segment *seg2;
				enum {
					start = 3,
					intsect = 2,
					end = 1
				} type;

				Event(const Point& xp, decltype(seg1) xseg1, decltype(seg2) xseg2);

				Event &operator=(const Event &e);

				bool operator<(const Event& that) const;
			};

			class EventQueue : public std::priority_queue<Event> {
			private:
				Point *sweepLine;
			public:
				void push(value_type &val);

				void setSweepLine(decltype(sweepLine) sl) { sweepLine = sl; }
			};

			struct CompSegPos {
			private:
				Point *sweepLine;

				long double getIntX(const Segment &seg);

			public:
				void setSweepLine(decltype(sweepLine) sl) { sweepLine = sl; }

				bool operator()(const Segment *a, const Segment *b);
			};

			class SweepLineStatus : public std::set<const Segment*, CompSegPos> {
			private:
				Point *sweepLine;
			public:
				void setSweepLine(decltype(sweepLine) sl) {
					sweepLine = sl;
					key_comp().setSweepLine(sl);
				}


			};
		public:
			static std::vector<std::tuple<Point, size_t, size_t>> compute(const std::vector<Segment> &segs);
		};
	public:
		/// <summary> Bentley-Ottmann Sweep algorithm </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		/// <remarks> This implementation uses a top-down sweep line </remarks>
		static std::vector<std::tuple<Point, size_t, size_t>> BOSweep(const std::vector<Segment> &segs);

		/// <summary> Brute-Force method </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		static std::vector<std::tuple<Point, size_t, size_t>> BruteForce(const std::vector<Segment> &segs);
	};
}