#pragma once
#include "Intersection.h"

namespace wrapper {
	public ref class Intersection {
	public:
		delegate void CSCallbackType(double,
			System::Collections::Generic::List<System::UInt32>^,
			System::Collections::Generic::List<System::UInt32>^);

		static System::Collections::Generic::List<System::Tuple<System::Drawing::PointF, System::UInt32, System::UInt32>^> ^
			getIntersection(System::Collections::Generic::List<System::Drawing::PointF> ^, CSCallbackType^);
	private:
		static CSCallbackType ^csCallback;

		static void wCallback(long double y, const std::vector<size_t>&, const std::vector<size_t>&);
	};
}
