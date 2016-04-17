#pragma once
#include "Intersection.h"

namespace wrapper {
	public ref class Intersection {
	private:
	public:
		delegate void CSCallbackType();

		static System::Collections::Generic::List<System::Tuple<System::Drawing::PointF, System::UInt32, System::UInt32>^> ^
			getIntersection(System::Collections::Generic::List<System::Drawing::PointF> ^, CSCallbackType^);
	};
}
