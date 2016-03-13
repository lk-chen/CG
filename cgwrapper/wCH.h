#pragma once
#include "point.h"

using namespace System;

namespace wrapper {
	public ref class CH {
	private:
		static std::vector<clk::Point> getPointsFromCoord(
			array<long double>^, array<long double>^);
	public:
		static array<size_t> ^incremental(
			array<long double>^, array<long double>^);

		static array<size_t> ^giftWrapping(
			array<long double>^, array<long double>^);
	};
}