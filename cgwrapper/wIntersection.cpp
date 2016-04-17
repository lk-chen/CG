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
		List<PointF> ^endPoints, CSCallbackType^ cb)
	{
		vector<Segment> segs;
		for (int i = 0; i < endPoints->Count; i+=2)
			segs.push_back(Segment(
				Point(endPoints[i].X,endPoints[i].Y),
				Point(endPoints[i + 1].X, endPoints[i + 1].Y)));

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

	void Intersection::wCallback(long double y, int eventi, int eventj, int slopeIdx, const vector<size_t>& SLSidx, int nexti, int nextj) {
		auto SLSIdx = gcnew List<UInt32>;
		for (auto idx : SLSidx)
			SLSIdx->Add(idx);
		csCallback(y, eventi, eventj, slopeIdx, SLSIdx, nexti, nextj);
	}
}
