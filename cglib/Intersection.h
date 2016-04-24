#pragma once
#include "point.h"
#include "segment.h"
#include <set>
#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>

namespace clk {
	/// <summary> Intersection <summary>
	class DLLEXP Intersection {
	public:
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

	private:
		/// <summary> Detect if a point is on another segment </summary>
		/// <param name="p"> Point </param>
		/// <param name="seg"> Segment </param>
		/// <returns> Returns true if point p lies on segment seg or
		/// overlap with endpoints of seg </returns>
		static bool pointOnSegment(const Point &p, const Segment &seg);

		class BOSweepClass {
		private:
			class EventQueue {
			private:
				const Point &sweepPoint;

				struct CompBySlope {
					bool operator()(const Segment*a, const Segment*b) {
						if ((*a).compareSlope(*b)) return true;
						else if ((*b).compareSlope(*a)) return false;
						else return a < b;
					}
				};
				struct HigherThenLeft {
					bool operator()(const Point& a, const Point& b) {
						if (a.y != b.y) return a.y < b.y;
						else return a.x > b.x;
					}
				};
				std::map<Point, std::set<const Segment*, CompBySlope>, HigherThenLeft> pq;
			public:
				void push(const Point &xp, const Segment* seg1, const Segment* seg2 = nullptr);

				const decltype(*pq.rbegin()) & top() { return (*pq.rbegin()); };

				void pop() { pq.erase(top().first); };

				bool empty() { return pq.empty(); };

				size_t size() { return pq.size(); }

				EventQueue(Point &_sp) :sweepPoint(_sp) {};
			};

			struct SegmentPosition {
				const Segment &seg;
				const Point &sweepPoint;
				const bool &reverse;

				SegmentPosition(const Segment &_seg, const Point &_sp, const bool &_r) :
					seg(_seg), sweepPoint(_sp), reverse(_r) {};

				bool operator<(const SegmentPosition &b) const;

			private:
				long double _intX() const;
			};
		public:
			template<class _Pr>
			static std::vector<std::tuple<Point, size_t, size_t>> compute(
				const std::vector<Segment> &segs, _Pr callback) {
				std::vector<std::tuple<Point, size_t, size_t>> intPoints;
				if (segs.size() == 0) return intPoints;
				Point sweepPoint;
				bool reverse = false;
				std::set<SegmentPosition> SLS;
				EventQueue EQ(sweepPoint);
				Point intPoint;

				std::ofstream fout("cglib.txt");

				for (const auto &seg : segs) {
					sweepPoint = seg.first;
					EQ.push(seg.first, &seg);
					EQ.push(seg.second, &seg);
					fout << seg.first.toString() << seg.second.toString() << '\n';
					fout << EQ.size() << '\n';
				}

				while (!EQ.empty())
				{
					const auto &event = EQ.top();
					decltype(SLS.begin()) it1, it2, it3, it4;
					std::vector<const Segment*> upper, lower, contain;
					sweepPoint = event.first;
					auto sl = *event.second.begin();
					auto sr = *event.second.rbegin();
					reverse = false;

					for (auto seg : event.second) {
						SLS.erase(SegmentPosition(*seg, sweepPoint, reverse));
						if (seg->first == sweepPoint) lower.push_back(seg);
						else if (seg->second == sweepPoint) upper.push_back(seg);
						else contain.push_back(seg);
					}

					reverse = true;
					for (auto seg : lower) SLS.insert(SegmentPosition(*seg, sweepPoint, reverse));
					for (auto seg : contain) SLS.insert(SegmentPosition(*seg, sweepPoint, reverse));

					it2 = SLS.lower_bound(SegmentPosition(*sr, sweepPoint, reverse));
					it4 = SLS.upper_bound(SegmentPosition(*sl, sweepPoint, reverse));

					if (it2 != SLS.begin() && it2 != SLS.end()) {
						it1 = it2; it1--;
						if (segmentIntersect(it1->seg, it2->seg, intPoint))
							EQ.push(intPoint, &it1->seg, &it2->seg);
					}

					if (it4 != SLS.begin() && it4 != SLS.end()) {
						it3 = it4; it3--;
						if (segmentIntersect(it3->seg, it4->seg, intPoint))
							EQ.push(intPoint, &it3->seg, &it4->seg);
					}

					for (auto it1 = event.second.begin(); it1 != event.second.end(); it1++)
						for (auto it2 = it1; it2 != event.second.end(); it2++)
							if (it2 == it1) continue;
							else intPoints.push_back(std::make_tuple(event.first,
								*it1 - &segs[0], *it2 - &segs[0]));

					vector<size_t> SLSIdx, eventIdx;
					for (auto it : SLS) SLSIdx.push_back(&it.seg - &segs[0]);
					for (auto it : event.second) eventIdx.push_back(it - &segs[0]);
					callback(sweepPoint.y, eventIdx, SLSIdx);

					EQ.pop();
				}

				return intPoints;
			}
		};
	public:
		/// <summary> Bentley-Ottmann Sweep algorithm </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="callback"> Add callback for animation </param>
		/// <remarks> This implementation uses a top-down sweep line </remarks>
		template<class _Pr>
		static std::vector<std::tuple<Point, size_t, size_t>> BOSweep(
			const std::vector<Segment> &segs, _Pr callback) {
			return Intersection::BOSweepClass::compute(segs, callback);
		}

		/// <summary> Brute-Force method </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		static std::vector<std::tuple<Point, size_t, size_t>> BruteForce(const std::vector<Segment> &segs);
	};
}