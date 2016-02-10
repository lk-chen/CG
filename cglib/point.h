#pragma once

#define DLLEXP __declspec(dllexport)

namespace clk {
	class DLLEXP Point {
	private:
		long double _x, _y;
	public:
		Point(long double, long double);
		~Point();
	};
}