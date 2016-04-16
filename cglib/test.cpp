#include <iostream>
#include "Intersection.h"

int main() {
	std::cout << "C++ test\n";
	std::vector<clk::Segment> segs;

	segs.push_back(clk::Segment(clk::Point(21, 22), clk::Point(8934, 9998)));
	clk::Intersection::BOSweep(segs);

	int a;
	std::cin >> a;

	return 0;
}