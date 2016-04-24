#include "stdafx.h"
#include <CppUnitTest.h>
#include "Intersection.h"
#include "pointTest.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace clk;

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

		//TEST_METHOD()
	};
}