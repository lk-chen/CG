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

		static void nullCallback(long double y, const std::vector<size_t> &eventIdx, const std::vector<size_t>& SLSIdx) { }

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
					bool operator()(const Point& a, const Point& b) const {
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
				vector<tuple<Point, size_t, size_t>> intPoints;
				if (segs.size() == 0) return intPoints;
				Point sweepPoint;
				bool reverse = false;
				set<SegmentPosition> SLS;
				EventQueue EQ(sweepPoint);
				Point intPoint;

				for (const auto &seg : segs) {
					sweepPoint = seg.first;
					EQ.push(seg.first, &seg);
					EQ.push(seg.second, &seg);
				}

				while (!EQ.empty())
				{
					const auto &event = EQ.top();
					decltype(SLS.begin()) it1, it2;
					vector<const Segment*> upper, lower, contain;
					sweepPoint = event.first;
					auto sl = *event.second.begin();
					auto sr = *event.second.rbegin();
					reverse = false;

					for (auto hline = event.second.begin(); hline != event.second.end(); ++hline) {
						if ((*hline)->isHorizontal()) {
							for (auto it = SLS.upper_bound(SegmentPosition(**hline, sweepPoint, reverse));
								it != SLS.end(); ++it) {
								if (segmentIntersect(**hline, it->seg, intPoint))
									EQ.push(intPoint, *hline, &it->seg);
								else break;
							}
						}
					}

					for (auto pseg : event.second) {
						SLS.erase(SegmentPosition(*pseg, sweepPoint, reverse));
						if (pseg->first == sweepPoint) lower.push_back(pseg);
						else if (pseg->second == sweepPoint) upper.push_back(pseg);
						else contain.push_back(pseg);
					}

					reverse = true;
					for (auto pseg : lower) SLS.insert(SegmentPosition(*pseg, sweepPoint, reverse));
					for (auto pseg : contain) SLS.insert(SegmentPosition(*pseg, sweepPoint, reverse));

					it2 = SLS.lower_bound(SegmentPosition(*sr, sweepPoint, reverse));
					if (it2 != SLS.begin() && it2 != SLS.end()) {
						it1 = it2; --it1;
						for (auto r = it2; r != SLS.end(); ++r) {
							if (r->seg.compareSlope(it2->seg) || it2->seg.compareSlope(r->seg)) break;
							for (auto l = it1; l != SLS.end(); --l) {
								if (l->seg.compareSlope(it1->seg) || it1->seg.compareSlope(l->seg)) break;
								if (segmentIntersect(l->seg, r->seg, intPoint))
									EQ.push(intPoint, &l->seg, &r->seg);
							}
						}
					}

					it2 = SLS.upper_bound(SegmentPosition(*sl, sweepPoint, reverse));
					if (it2 != SLS.begin() && it2 != SLS.end()) {
						it1 = it2; --it1;
						for (auto r = it2; r != SLS.end(); ++r) {
							if (r->seg.compareSlope(it2->seg) || it2->seg.compareSlope(r->seg)) break;
							for (auto l = it1; l != SLS.end(); --l) {
								if (l->seg.compareSlope(it1->seg) || it1->seg.compareSlope(l->seg)) break;
								if (segmentIntersect(l->seg, r->seg, intPoint))
									EQ.push(intPoint, &l->seg, &r->seg);
							}
						}
					}

					for (auto it1 = event.second.begin(); it1 != event.second.end(); ++it1)
						for (auto it2 = it1; ; ) {
							++it2;
							if (it2 == event.second.end()) break;

							segmentIntersect(**it1, **it2, intPoint);
							if (intPoint == event.first)
								intPoints.push_back(make_tuple(event.first,
									*it1 - &segs[0], *it2 - &segs[0]));
						}

					std::vector<size_t> SLSIdx, eventIdx;
					for (auto it : SLS) SLSIdx.push_back(&it.seg - &segs[0]);
					for (auto it : event.second) eventIdx.push_back(it - &segs[0]);
					callback(sweepPoint.y, eventIdx, SLSIdx);

					EQ.pop();
				}

				return intPoints;
			}

			static std::vector<std::tuple<Point, size_t, size_t>> compute(
				const std::vector<Segment> &segs);
		};
	public:
		/// <summary> Bentley-Ottmann Sweep algorithm </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="callback"> Add callback for animation </param>
		/// <remarks> This implementation uses a top-down sweep line </remarks>
		template<class _Pr>
		static std::vector<std::tuple<Point, size_t, size_t>> BOSweep(
			const std::vector<Segment> &segs, _Pr callback) {
			//return Intersection::BOSweepClass::compute(segs, callback);
			return Intersection::BOSweepClass::compute(segs, callback);
		}

		/// <summary> Bentley-Ottmann Sweep algorithm </summary>
		/// <param name="segs"> Input segments to be detected </param>
		static std::vector<std::tuple<Point, size_t, size_t>> BOSweep(
			const std::vector<Segment> &segs) {
			return Intersection::BOSweepClass::compute(segs);
		}

		/// <summary> Brute-Force method </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		static std::vector<std::tuple<Point, size_t, size_t>> BruteForce(const std::vector<Segment> &segs);
	};
}