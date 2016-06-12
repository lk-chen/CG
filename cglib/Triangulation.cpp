#include "Triangulation.h"
#include "CH.h"

using namespace std;

vector<tuple<size_t, size_t>> clk::Triangulation::monotoneTriangulation(const vector<Point>& vertices)
{
	vector<tuple<size_t, size_t>> res;

	auto ltl = CH::lowestThenLeftmost(vertices);

	return res;
}
