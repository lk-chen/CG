#pragma once
#include "triangulation.h"

namespace wrapper {
	public ref class Triangulation {
	public:
		static System::Collections::Generic::List<System::Tuple<System::UInt32, System::UInt32>^>^
			triangulation(System::Collections::Generic::List<System::Drawing::PointF> ^);
	};
}