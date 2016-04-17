#include "wIntersection.h"
#include "Intersection.h"
#include <tuple>

using std::vector;
using std::get;
using clk::Segment;
using clk::Point;
using System::Collections::Generic::List;
using System::Drawing::PointF;
using System::UInt32;
using System::IntPtr;
using System::Runtime::InteropServices::Marshal;
typedef System::Tuple<PointF, UInt32, UInt32> IntPointTuple;

namespace wrapper {
	List<IntPointTuple^> ^Intersection::getIntersection(
		List<PointF> ^endPoints, CSCallbackType^ csCallback)
	{
		vector<Segment> segs;
		for (int i = 0; i < endPoints->Count; i+=2)
			segs.push_back(Segment(
				Point(endPoints[i].X,endPoints[i].Y),
				Point(endPoints[i + 1].X, endPoints[i + 1].Y)));

		IntPtr csCbptr = Marshal::GetFunctionPointerForDelegate(csCallback);
		clk::Intersection::CallbackType callback = static_cast<clk::Intersection::CallbackType>(csCbptr.ToPointer());
		auto intPoints = clk::Intersection::BOSweep(segs, callback);

		auto res = gcnew List<IntPointTuple^>;
		for (size_t i = 0; i < intPoints.size(); i++)
		{
			auto point = get<0>(intPoints[i]);
			res->Add(gcnew IntPointTuple(PointF(point.x, point.y),
				get<1>(intPoints[i]), get<2>(intPoints[i])));
		}

		return res;
	}
}