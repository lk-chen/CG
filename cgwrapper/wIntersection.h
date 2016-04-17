#pragma once
#include "Intersection.h"
#using <System.Drawing.dll>

namespace wrapper {
	public ref class Intersection {
	private:
	public:
		static System::Collections::Generic::List<System::Tuple<System::Drawing::PointF, System::UInt32, System::UInt32>^> ^
			getIntersection(System::Collections::Generic::List<System::Drawing::PointF> ^);
	};
}
