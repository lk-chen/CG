#include "wIntersection.h"
#include "Intersection.h"
#include <tuple>
#include <fstream>

using std::ofstream;
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
		List<PointF> ^endPoints, CSCallbackType^ cb)
	{
		vector<Segment> segs(endPoints->Count / 2);
		for (int i = 0; i < endPoints->Count; i += 2) {
			segs[i / 2] = Segment(
				Point(endPoints[i].X, endPoints[i].Y),
				Point(endPoints[i + 1].X, endPoints[i + 1].Y));
		}

		csCallback = cb;
		auto intPoints = clk::Intersection::BOSweep(segs, wCallback);

		auto res = gcnew List<IntPointTuple^>;
		for (size_t i = 0; i < intPoints.size(); i++)
		{
			auto point = get<0>(intPoints[i]);
			res->Add(gcnew IntPointTuple(PointF(point.x, point.y),
				get<1>(intPoints[i]), get<2>(intPoints[i])));
		}

		return res;
	}

	void Intersection::wCallback(long double y, const vector<size_t>& eventidx, const vector<size_t>& SLSidx) {
		auto SLSIdx = gcnew List<UInt32>;
		auto eventIdx = gcnew List<UInt32>;
		for (auto idx : SLSidx)
			SLSIdx->Add(idx);
		for (auto idx : eventidx)
			eventIdx->Add(idx);
		csCallback(y, eventIdx, SLSIdx);
	}
}
