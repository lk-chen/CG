#include "Intersection.h"
#include <iostream>

using namespace std;
using namespace clk;

void callback(long double y, const vector<size_t> eventIdx, const vector<size_t>& SLSIdx) { }

int main() {
	vector<Segment> segs;
	segs.push_back(Segment(Point(285, 91), Point(230, 187)));
	segs.push_back(Segment(Point(241, 107), Point(313, 249)));
	segs.push_back(Segment(Point(307, 124), Point(266, 223)));

	auto res = Intersection::BOSweep(segs, callback);
}