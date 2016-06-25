#include "triangulation.h"
#include "CH.h"
#include <stdexcept>

using namespace std;

vector<tuple<size_t, size_t>> clk::Triangulation::monotoneTriangulation(const vector<Point>& vertices)
{
	vector<tuple<size_t, size_t>> res;
	auto sz = vertices.size();
	if (sz < 3) throw invalid_argument("too few vertices");

	auto ltl = CH::lowestThenLeftmost(vertices);
	auto l = (ltl - 1 + sz) % sz, r = (ltl + 1) % sz;
	bool baseOnLeft;
	vector<size_t> stack;
	stack.push_back(ltl);

	while (l != r) {
		bool curOnLeft = vertices[l].y < vertices[r].y;
		if (stack.size() < 2) {
			stack.push_back(curOnLeft ? l : r);
			baseOnLeft = curOnLeft;
		}
		else {
			if (baseOnLeft == curOnLeft) {
				bool reflex = curOnLeft ?
					vertices[l].toLeft(vertices[stack[stack.size() - 2]], vertices[stack.back()]) :
					vertices[r].toRight(vertices[stack[stack.size() - 2]], vertices[stack.back()]);
				if (reflex) {
					stack.push_back(curOnLeft ? l : r);
				}
				else {
					if (curOnLeft) {
						while (stack.size() >= 2 &&
							vertices[l].toRight(vertices[stack[stack.size() - 2]], vertices[stack.back()]) &&
							!vertices[l].toLeft(vertices[stack[stack.size() - 2]], vertices[stack.back()])) {
							stack.pop_back();
							res.push_back(make_tuple(l, stack.back()));
						}
						stack.push_back(l);
					}
					else {
						while (stack.size() >= 2 &&
							vertices[r].toLeft(vertices[stack[stack.size() - 2]], vertices[stack.back()]) &&
							!vertices[r].toRight(vertices[stack[stack.size() - 2]], vertices[stack.back()])) {
							stack.pop_back();
							res.push_back(make_tuple(r, stack.back()));
						}
						stack.push_back(r);
					}
				}
			}
			else {
				auto newBase = stack.back();
				while (stack.size() > 1) {
					res.push_back(make_tuple(stack.back(),
						curOnLeft ? l : r));
					stack.pop_back();
				}
				stack[0] = newBase;
				stack.push_back(curOnLeft ? l : r);
				baseOnLeft = curOnLeft;
			}
		}

		if (curOnLeft)
			l = (l - 1 + sz) % sz;
		else
			r = (r + 1) % sz;
	}

	stack.pop_back();
	for (auto it = stack.begin() + 1; it != stack.end(); ++it)
		res.push_back(make_tuple(l, *it));

	return res;
}
