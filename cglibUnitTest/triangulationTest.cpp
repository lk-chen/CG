#include "stdafx.h"
#include <CppUnitTest.h>
#include <algorithm>
#include <random>
#include "triangulation.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace clk;
using namespace std;

namespace cglibUnitTest
{
	TEST_CLASS(IntersectionTest)
	{
	public:

		TEST_METHOD(MonotoneTriangulationCommonReflexTest) {
			vector<Point> vertices;
			vertices.push_back(Point());
			vertices.push_back(Point(-0.5, 3));
			vertices.push_back(Point(-1, 1.5));
			vertices.push_back(Point(-1.5, 1));
			vertices.push_back(Point(-3, 0.5));
			auto res = Triangulation::monotoneTriangulation(vertices);
			Assert::AreEqual((size_t)2, res.size());

			vector<size_t> idxVec, correctIdxVec;
			for (auto t : res) {
				idxVec.push_back(get<0>(t));
				idxVec.push_back(get<1>(t));
			}
			sort(idxVec.begin(), idxVec.end());
			correctIdxVec = { 0,0,2,3 };
			for (size_t i = 0; i < idxVec.size(); ++i)
				Assert::AreEqual(idxVec[i], correctIdxVec[i]);

			auto tmp(vertices);
			for (auto &p : tmp)
				p = Point(-p.x, -p.y);
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(tmp).size());

			reverse(tmp.begin(), tmp.end());
			auto p1 = tmp[0];
			auto p2 = tmp[1];
			auto p3 = tmp[2];
			auto p4 = tmp[3];
			for (auto &p : tmp)
				p = Point(-p.x, p.y);
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(tmp).size());

			for (auto &p : tmp)
				p = Point(-p.x, -p.y);
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(tmp).size());
		}

		TEST_METHOD(MonotoneTriangulationCommonConvexTest) {
			vector<Point> vertices;
			vertices.push_back(Point());
			vertices.push_back(Point(-0.5, 3));
			vertices.push_back(Point(-2, 2.5));
			vertices.push_back(Point(-2.5, 2));
			vertices.push_back(Point(-3, 0.5));
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(vertices).size());

			auto tmp(vertices);
			for (auto &p : tmp)
				p = Point(-p.x, -p.y);
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(vertices).size());

			reverse(tmp.begin(), tmp.end());
			for (auto &p : tmp)
				p = Point(-p.x, p.y);
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(vertices).size());

			for (auto &p : tmp)
				p = Point(-p.x, -p.y);
			Assert::AreEqual((size_t)2, Triangulation::monotoneTriangulation(vertices).size());
		}

		TEST_METHOD(MonotoneTriangulationCollinearTest) {

		}

		TEST_METHOD(MonotoneTriangulationRandomPerformanceTest) {
			auto sz = 1000;
			vector<Point> vertices;
			float y = 0;
			vertices.push_back(Point(0, y));
			for (auto i = 0; i < sz; ++i) {
				bool putOnLeft = rand() % 2 == 0;
				y += (float)rand() / RAND_MAX;
				vertices.push_back(Point(
					putOnLeft ? -((float)rand() / RAND_MAX) : ((float)rand() / RAND_MAX),
					y));
			}
			y += (float)rand() / RAND_MAX;
			vertices.push_back(Point(0, y));

			Assert::AreEqual(vertices.size() - 3, Triangulation::monotoneTriangulation(vertices).size());
		}
	};
}