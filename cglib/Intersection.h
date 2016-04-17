#pragma once
#include "point.h"
#include "segment.h"
#include <set>
#include <vector>
#include <tuple>
#include <functional>

namespace clk {
	/// <summary> Intersection <summary>
	class DLLEXP Intersection {
	public:
		/// <summary> Call back funtion type </summary>
		typedef void(*CallbackType)(long double, int, int, int, const std::vector<size_t>&, int, int);

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

		class BOSweepClass {
		private:
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

			class EventQueue {
			private:
				const Point &sweepPoint;
				const Segment &sweepSlope;
				std::set<Event> pq;
			public:
				void push(Event &val);

				const Event& top() const { return *(pq.rbegin()); };

				void pop() { pq.erase(top()); };

				bool empty() { return pq.empty(); };

				EventQueue(Point &_sp, Segment &_ss) :sweepPoint(_sp), sweepSlope(_ss) {};
			};

			struct SegmentPosition {
				const Segment &seg;
				const Point &sweepPoint;
				const Segment &sweepSlope;

				SegmentPosition(const Segment &_seg, const Point &_sp, const Segment &_ss) :
					seg(_seg), sweepPoint(_sp), sweepSlope(_ss) {};
			};

			struct CompSegPos {
			private:
				long double _intX(const SegmentPosition &seg);

			public:
				bool operator()(const SegmentPosition &a, const SegmentPosition &b);
			};

			class SweepLineStatus : public std::set<SegmentPosition, CompSegPos> {
			private:
			public:
			};
		public:
			template<class _Pr>
			static std::vector<std::tuple<Point, size_t, size_t>> compute(const std::vector<Segment> &segs, _Pr callback) {
				std::vector<std::tuple<Point, size_t, size_t>> intPoints;
				if (segs.size() == 0) return intPoints;
				Point sweepPoint;
				Segment sweepSlope(segs[0].first, segs[0].second);
				SweepLineStatus SLS;
				EventQueue EQ(sweepPoint, sweepSlope);
				Point intPoint;

				for (auto &seg : segs) {
					EQ.push(Event(seg.first, &seg, nullptr));
					EQ.push(Event(seg.second, nullptr, &seg));
				}

				while (!EQ.empty())
				{
					size_t slopeIdx, eventi, eventj, nexti, nextj;
					auto event = EQ.top();
					decltype(SLS.begin()) it1, it2, it3, it4;
					sweepPoint = event.p;

					if (event.type == Event::start) {
						sweepSlope = *event.seg1;
						slopeIdx = event.seg1 - &segs[0];
						SegmentPosition sp(*event.seg1, sweepPoint, sweepSlope);
						SLS.insert(sp);
						it1 = it2 = it3 = it4 = SLS.find(sp);
						it1--;
						it4++;
					}
					else if (event.type == Event::end) {
						sweepSlope = *event.seg2;
						slopeIdx = event.seg2 - &segs[0];
						SegmentPosition sp(*event.seg2, sweepPoint, sweepSlope);
						SLS.erase(sp);
						it1 = it2 = it3 = it4 = SLS.upper_bound(sp);
						if (!SLS.empty()) {
							it1--;
							it3--;
						}
					}
					else {
						auto i = event.seg1 - &segs[0];
						auto j = event.seg2 - &segs[0];
						intPoints.push_back(std::make_tuple(event.p, i, j));
						sweepSlope = event.seg1->compareSlope(*event.seg2)
							? *event.seg1 : *event.seg2;
						SegmentPosition sp1(*event.seg1, sweepPoint, sweepSlope);
						SegmentPosition sp2(*event.seg2, sweepPoint, sweepSlope);
						SLS.erase(sp1);
						SLS.erase(sp2);
						SegmentPosition sp(event.seg1->compareSlope(*event.seg2)
							? *event.seg2 : *event.seg1, sweepPoint, sweepSlope);
						sweepSlope = sp.seg;
						slopeIdx = &sp.seg - &segs[0];
						SLS.insert(sp1);
						SLS.insert(sp2);
						it1 = it2 = it3 = it4 = SLS.find(sp);
						it1--;it1--;
						it2--;
						it4++;
					}

					if (it1 != SLS.end()
						&& it2 != SLS.begin() && it2 != SLS.end()
						&& segmentIntersect(it1->seg, it2->seg, intPoint))
						EQ.push(Event(intPoint, &it1->seg, &it2->seg));
					if (it3 != SLS.end()
						&& it4 != SLS.begin() && it4 != SLS.end()
						&& segmentIntersect(it3->seg, it4->seg, intPoint))
						EQ.push(Event(intPoint, &it3->seg, &it4->seg));

					EQ.pop();

					eventi = (event.seg1 == nullptr) ? -1 : event.seg1 - &segs[0];
					eventj = (event.seg2 == nullptr) ? -1 : event.seg2 - &segs[0];
					vector<size_t> SLSidx;
					for (auto it = SLS.begin(); it != SLS.end(); it++)
						SLSidx.push_back(&it->seg - &segs[0]);
					if (EQ.empty()) nexti = nextj = -1;
					else {
						event = EQ.top();
						nexti = (event.seg1 == nullptr) ? -1 : event.seg1 - &segs[0];
						nextj = (event.seg2 == nullptr) ? -1 : event.seg2 - &segs[0];
					}
					callback(sweepPoint.y, eventi, eventj, slopeIdx,
						SLSidx, nexti, nextj);
				}

				return intPoints;
			}
		};
	public:
		/// <summary> Bentley-Ottmann Sweep algorithm </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		/// <remarks> This implementation uses a top-down sweep line </remarks>
		template<class _Pr>
		static std::vector<std::tuple<Point, size_t, size_t>> BOSweep(const std::vector<Segment> &segs, _Pr callback) {
			return Intersection::BOSweepClass::compute(segs, callback);
		}

		/// <summary> Brute-Force method </summary>
		/// <param name="segs"> Input segments to be detected </param>
		/// <param name="strict"> If strict is set to be true, only
		/// strictly intersections are counted. </param>
		static std::vector<std::tuple<Point, size_t, size_t>> BruteForce(const std::vector<Segment> &segs);
	};
}