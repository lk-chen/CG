#include "stdafx.h"
#include <CppUnitTest.h>
#include "segment.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace clk;

namespace cglibUnitTest
{		
	TEST_CLASS(SegmentTest)
	{
	public:
		
		TEST_METHOD(Construct)
		{
			double px = 20, py = 40, qx = 30, qy = 30;
			Segment s1(Point(px, py), Point(qx, qy));
			Assert::AreEqual(py, (double)s1.first.y);
			Assert::AreEqual(0.0, (double)(s1.a * px + s1.b * py - s1.c));
			Assert::AreEqual(0.0, (double)(s1.a * qx + s1.b * qy - s1.c));
			px = 20, py = 30, qx = 30, qy = 30;
			Segment s2(Point(px, py), Point(qx, qy));
			Assert::AreEqual(px, (double)s2.first.x);
			Assert::AreEqual(0.0, (double)(s2.a * px + s2.b * py - s2.c));
			Assert::AreEqual(0.0, (double)(s2.a * qx + s2.b * qy - s2.c));
		}

		TEST_METHOD(CompareSlope)
		{
			Segment s1(Point(-100, 0), Point(0, 0));
			Segment s2(Point(-100, 100), Point(0, 0));
			Segment s3(Point(0, 100), Point(0, 0));
			Segment s4(Point(100, 100), Point(0, 0));
			Segment s5(Point(50, 0), Point(0, 0));
			Segment s6(Point(50, -100), Point(0, 0));
			Segment s7(Point(-50, -100), Point(0, 0));
			Segment s8(Point(-100, -50), Point(0, 0));

			Assert::IsFalse(s1.compareSlope(s1));
			Assert::IsTrue(s1.compareSlope(s2));
			Assert::IsTrue(s1.compareSlope(s3));
			Assert::IsTrue(s1.compareSlope(s4));
			Assert::IsFalse(s1.compareSlope(s5));
			Assert::IsTrue(s1.compareSlope(s6));
			Assert::IsTrue(s1.compareSlope(s7));
			Assert::IsTrue(s1.compareSlope(s8));

			Assert::IsFalse(s2.compareSlope(s1));
			Assert::IsFalse(s2.compareSlope(s2));
			Assert::IsTrue(s2.compareSlope(s3));
			Assert::IsTrue(s2.compareSlope(s4));
			Assert::IsFalse(s2.compareSlope(s5));
			Assert::IsTrue(s2.compareSlope(s6));
			Assert::IsTrue(s2.compareSlope(s7));
			Assert::IsTrue(s2.compareSlope(s8));

			Assert::IsFalse(s3.compareSlope(s1));
			Assert::IsFalse(s3.compareSlope(s2));
			Assert::IsFalse(s3.compareSlope(s3));
			Assert::IsTrue(s3.compareSlope(s4));
			Assert::IsFalse(s3.compareSlope(s5));
			Assert::IsFalse(s3.compareSlope(s6));
			Assert::IsTrue(s3.compareSlope(s7));
			Assert::IsTrue(s3.compareSlope(s8));

			Assert::IsFalse(s4.compareSlope(s1));
			Assert::IsFalse(s4.compareSlope(s2));
			Assert::IsFalse(s4.compareSlope(s3));
			Assert::IsFalse(s4.compareSlope(s4));
			Assert::IsFalse(s4.compareSlope(s5));
			Assert::IsFalse(s4.compareSlope(s6));
			Assert::IsFalse(s4.compareSlope(s7));
			Assert::IsTrue(s4.compareSlope(s8));

			Assert::IsFalse(s5.compareSlope(s1));
			Assert::IsTrue(s5.compareSlope(s2));
			Assert::IsTrue(s5.compareSlope(s3));
			Assert::IsTrue(s5.compareSlope(s4));
			Assert::IsFalse(s5.compareSlope(s5));
			Assert::IsTrue(s5.compareSlope(s6));
			Assert::IsTrue(s5.compareSlope(s7));
			Assert::IsTrue(s5.compareSlope(s8));

			Assert::IsFalse(s6.compareSlope(s1));
			Assert::IsFalse(s6.compareSlope(s2));
			Assert::IsTrue(s6.compareSlope(s3));
			Assert::IsTrue(s6.compareSlope(s4));
			Assert::IsFalse(s6.compareSlope(s5));
			Assert::IsFalse(s6.compareSlope(s6));
			Assert::IsTrue(s6.compareSlope(s7));
			Assert::IsTrue(s6.compareSlope(s8));

			Assert::IsFalse(s7.compareSlope(s1));
			Assert::IsFalse(s7.compareSlope(s2));
			Assert::IsFalse(s7.compareSlope(s3));
			Assert::IsTrue(s7.compareSlope(s4));
			Assert::IsFalse(s7.compareSlope(s5));
			Assert::IsFalse(s7.compareSlope(s6));
			Assert::IsFalse(s7.compareSlope(s7));
			Assert::IsTrue(s7.compareSlope(s8));

			Assert::IsFalse(s8.compareSlope(s1));
			Assert::IsFalse(s8.compareSlope(s2));
			Assert::IsFalse(s8.compareSlope(s3));
			Assert::IsFalse(s8.compareSlope(s4));
			Assert::IsFalse(s8.compareSlope(s5));
			Assert::IsFalse(s8.compareSlope(s6));
			Assert::IsFalse(s8.compareSlope(s7));
			Assert::IsFalse(s8.compareSlope(s8));

		}

	};
}