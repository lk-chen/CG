#pragma once
#include "Intersection.h"

using namespace System;

namespace wrapper {
	public ref class Intersection {
	private:
		static std::vector<clk::Segment> getSegsFromCoord(
			array<long double>^, array<long double>^);
	public:
		static array<long double> ^getIntersection(
			array<long double>^, array<long double>^);
	};
}
