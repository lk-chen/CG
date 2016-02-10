#pragma once
#include "point.h"

using namespace System;

namespace wrapper {
	public ref class CH {
	public:
		static array<size_t> ^incremental(
			array<long double>^, array<long double>^);
	};
}