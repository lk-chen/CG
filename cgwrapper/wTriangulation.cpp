#include "wTriangulation.h"
#include <vector>
#include <tuple>
#include "point.h"

using System::Collections::Generic::List;
using System::UInt32;
using System::Drawing::PointF;
using std::vector;
using std::get;
using clk::Point;
typedef System::Tuple<UInt32, UInt32> UIntTuple;

List<UIntTuple^> ^wrapper::Triangulation::triangulation(List<PointF>^ vertices)
{
	auto res = gcnew List<UIntTuple^>;
	vector<Point> _vertices(vertices->Count);
	for (auto i = 0;i < vertices->Count; ++i)
		_vertices[i] = Point(vertices[i].X, vertices[i].Y);

	auto _res = clk::Triangulation::monotoneTriangulation(_vertices);
	for (auto& uintTup : _res) {
		res->Add(gcnew UIntTuple(get<0>(uintTup), get<1>(uintTup)));
	}

	return res;
}
