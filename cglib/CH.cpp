#include "CH.h"
#include <algorithm>

namespace clk {
	void CH::getTangent(const size_t x, const std::vector<size_t>& polygon,
		const std::vector<DistinctPoint>& points, size_t & s, size_t & t)
	{
		if (polygon.size() <= 1) {
			s = t = 0;
			return;
		}

		bool setS = false, setT = false;
		size_t i = 0, pre = polygon.size() - 1, succ = 1;
		
		while (i < polygon.size()) {
			int leftFlag = 0x00;
			if (points[polygon[pre]].toLeft(points[x], points[polygon[i]])) leftFlag |= 0x10;
			if (points[polygon[succ]].toLeft(points[x], points[polygon[i]])) leftFlag |= 0x01;
			
			if (leftFlag == 0x11) {
				s = i;
				setS = true;
				if (setT) return;
			}
			else if (leftFlag == 0x00) {
				t = i;
				setT = true;
				if (setS) return;
			}

			pre = i;
			i++;
			succ = (succ + 1) % polygon.size();
		}

		s = t = polygon.size();
	}
	std::vector<DistinctPoint> CH::groupPoints(const std::vector<Point>& points)
	{
		std::vector<DistinctPoint> indexedPoints, dPoints;

		for (size_t i = 0; i < points.size(); i++) {
			indexedPoints.push_back(DistinctPoint(points[i]));
			indexedPoints[i].members.push_back(i);
		}

		struct {
			bool operator()(const Point &p, const Point &q) {
				if (p.X() < q.X()) return true;
				else if (p.X() > q.X()) return false;
				else {
					if (p.Y() < q.Y()) return true;
					else return false;
				}
			}
		} compareByXThenY;

		std::sort(indexedPoints.begin(), indexedPoints.end(), compareByXThenY);

		for (auto point : indexedPoints) {
			if (dPoints.empty()) dPoints.push_back(point);
			else {
				if (dPoints.back() == point) dPoints.back().members.push_back(point.members[0]);
				else dPoints.push_back(point);
			}
		}

		return dPoints;
	}
	std::vector<size_t> CH::incremental(const std::vector<Point>& points) {
		auto dPoints = groupPoints(points);
		size_t s, t;
		std::vector<size_t> polygon{ 0 };

		for (size_t x = 1; x < dPoints.size(); x++) {
			getTangent(x, polygon, dPoints, s, t);
			if (s < polygon.size() && t < polygon.size()) {
				decltype(polygon) temp;
				if (s == t) {
					temp.push_back(polygon[s]);
					temp.push_back(x);
				}
				else if (s < t) {
					while (s <= t)
						temp.push_back(polygon[s++]);
					temp.push_back(x);
				}
				else {
					while (s < polygon.size())
						temp.push_back(polygon[s++]);
					s = 0;
					while (s <= t)
						temp.push_back(polygon[s++]);
					temp.push_back(x);
				}
				polygon = temp;
			}
		}

		std::vector<size_t> EP;
		for (auto distinctVertex : polygon)
			for (auto vertex : dPoints[distinctVertex].members)
				EP.push_back(vertex);
		
		return EP;
	}
}