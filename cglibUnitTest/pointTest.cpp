#include "stdafx.h"
#include <CppUnitTest.h>
#include "point.h"
#include <codecvt>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace clk;

namespace Microsoft{
	namespace VisualStudio {
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<Point>(const Point &p) {
				std::wstring ws;
				auto ns = p.toString();
				ws.assign(ns.begin(), ns.end());
				return ws;
			}
		}
	}
}