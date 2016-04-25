#include "stdafx.h"
#include <CppUnitTest.h>
#include "Intersection.h"
#include "pointTest.cpp"
#include <vector>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace clk;
using namespace std;

namespace cglibUnitTest
{
	TEST_CLASS(IntersectionTest)
	{
	public:

		TEST_METHOD(SegmentIntersectionTest)
		{
			Segment s1(Point(0, 100), Point(100, 0));
			Segment s2(Point(100, 0), Point(200, 0));
			Segment s3(Point(0, 0), Point(100, 200));
			Point p1, p2;
			Assert::IsTrue(Intersection::segmentIntersect(s1, s2, p1));
			Assert::AreEqual(Point(100, 0), p1);
			Assert::IsTrue(Intersection::segmentIntersect(s1, s2, p1));
			Assert::IsTrue(Intersection::segmentIntersect(s2, s1, p2));
			Assert::AreEqual(p1, p2);
		}

		TEST_METHOD(BOSweepGeneralCaseTest)
		{
			vector<Segment> segs;
			segs.push_back(Segment(Point(0, 0), Point(100, 0)));
			segs.push_back(Segment(Point(0, -50), Point(50, 100)));
			segs.push_back(Segment(Point(0, 50), Point(100, -50)));
			auto res = Intersection::BOSweep(segs);
			Assert::AreEqual(3, (int)res.size());
		}

		TEST_METHOD(BOSweepEventsSameLineCaseTest)
		{
			vector<Segment> segs;
			segs.push_back(Segment(Point(0, -50), Point(50, 50)));
			segs.push_back(Segment(Point(0, 50), Point(50, -50)));
			segs.push_back(Segment(Point(100, -50), Point(150, 0)));
			segs.push_back(Segment(Point(200, -50), Point(150, 0)));
			segs.push_back(Segment(Point(200, 50), Point(250, 0)));
			segs.push_back(Segment(Point(300, 50), Point(250, 0)));
			segs.push_back(Segment(Point(300, -50), Point(350, 0)));
			segs.push_back(Segment(Point(400, 50), Point(350, 0)));
			segs.push_back(Segment(Point(450, 50), Point(450, 0)));
			segs.push_back(Segment(Point(450, -50), Point(450, 0)));
			auto res = Intersection::BOSweep(segs);
			Assert::AreEqual(5, (int)res.size());
		}

		TEST_METHOD(BOSweepSameIntersectPointCaseTest)
		{
			vector<Segment> segs;
			segs.push_back(Segment(Point(-100, 0), Point(100, 0)));
			segs.push_back(Segment(Point(-100, 50), Point(100, -50)));
			segs.push_back(Segment(Point(-100, -50), Point(100, 50)));
			segs.push_back(Segment(Point(-0, 100), Point(0, -100)));
			segs.push_back(Segment(Point(0, 0), Point(3414, 97)));
			auto res = Intersection::BOSweep(segs);
			Assert::AreEqual(10, (int)res.size());

			segs.resize(0);
			segs.push_back(Segment(Point(10, 10), Point(20, 20)));
			segs.push_back(Segment(Point(10, 10), Point(20, 20)));
			segs.push_back(Segment(Point(10, 10), Point(20, 20)));
			segs.push_back(Segment(Point(10, 20), Point(20, 10)));
			segs.push_back(Segment(Point(10, 20), Point(20, 10)));
			res = Intersection::BOSweep(segs);
			Assert::AreEqual(10, (int)res.size());

		}

		TEST_METHOD(BOSweepHorizontalCaseTest)
		{
			vector<Segment> segs;
			segs.push_back(Segment(Point(-0, -0), Point(100, 0)));
			segs.push_back(Segment(Point(-100, 100), Point(-40, -100)));
			segs.push_back(Segment(Point(0, 500), Point(0, 0)));
			segs.push_back(Segment(Point(10, 100), Point(20, -3)));
			segs.push_back(Segment(Point(30, -20), Point(40, 20)));
			segs.push_back(Segment(Point(50, 23), Point(50, -31)));
			segs.push_back(Segment(Point(101, 30), Point(101, -30)));
			auto res = Intersection::BOSweep(segs);
			Assert::AreEqual(4, (int)res.size());

			segs.push_back(Segment(Point(-0, -0), Point(100, 0)));
			res = Intersection::BOSweep(segs);
			Assert::AreEqual(9, (int)res.size());
		}

		TEST_METHOD(BOSweepParallelCaseTest)
		{
			vector<Segment> segs;
			segs.push_back(Segment(Point(-100, 100), Point(-100, 0)));
			segs.push_back(Segment(Point(-90, 100), Point(-90, 0)));
			segs.push_back(Segment(Point(-100, 100), Point(-100, 20)));
			auto res = Intersection::BOSweep(segs);
			Assert::AreEqual(1, (int)res.size());
			segs.push_back(Segment(Point(-100, 50), Point(-200, 20)));
			res = Intersection::BOSweep(segs);
			Assert::AreEqual(3, (int)res.size());

			segs.resize(0);
			segs.push_back(Segment(Point(-100, 100), Point(100, 100)));
			segs.push_back(Segment(Point(-100, 30), Point(100, 30)));
			segs.push_back(Segment(Point(-100, 100), Point(130, 100)));
			res = Intersection::BOSweep(segs);
			Assert::AreEqual(1, (int)res.size());
			segs.push_back(Segment(Point(0, 50), Point(-0, 200)));
			res = Intersection::BOSweep(segs);
			Assert::AreEqual(3, (int)res.size());

			segs.resize(0);
			segs.push_back(Segment(Point(123, 50), Point(0, 0)));
			segs.push_back(Segment(Point(246, 100), Point(-123, -50)));
			res = Intersection::BOSweep(segs);
			Assert::AreEqual(1, (int)res.size());
		}

		TEST_METHOD(BOSweepRandomDataTest)
		{
			int sz = 500;
			vector<Segment> segs(sz+1);
			for (int i = 0; i < sz; i++) {
				segs[i] = Segment(Point(rand(), rand())
					, Point(rand(), rand()));
			}
			segs[sz] = segs[sz - 1];
			Assert::AreEqual(Intersection::BruteForce(segs).size()
				, Intersection::BOSweep(segs).size());
		}
	};
}