#include "Intersection.h"
#include <iostream>

using namespace std;
using namespace clk;

int main() {
	vector<Segment> segs;
	/*segs.push_back(Segment(Point(1, 1), Point(2, 2)));
	segs.push_back(Segment(Point(1, 1), Point(2, 2)));
	segs.push_back(Segment(Point(1, 1), Point(2, 2)));
	segs.push_back(Segment(Point(2, 1), Point(1, 2)));*/
	segs.push_back(Segment(Point(123, 50), Point(0, 0)));
	segs.push_back(Segment(Point(246, 100), Point(-123, -50)));

	auto res = Intersection::BOSweep(segs);
}