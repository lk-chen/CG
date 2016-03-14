#include "CH.h"
#include <algorithm>
#include <functional>

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

	std::vector<size_t> CH::deGroupPoints(const std::vector<DistinctPoint>& dPoints, const std::vector<size_t>& polygon) {
		std::vector<size_t> EP;
		for (auto distinctVertex : polygon)
			for (auto vertex : dPoints[distinctVertex].members)
				EP.push_back(vertex);

		return EP;
	}

	size_t CH::LTL(const std::vector<DistinctPoint>& points)
	{
		struct {
			bool operator()(const DistinctPoint &p, const DistinctPoint &q) {
				if (p.Y() < q.Y()) return true;
				else if (p.Y() > q.Y()) return false;
				else {
					if (p.X() < q.X()) return true;
					else if (p.X() > q.X()) return false;
					else {
						throw "Repeated points";
					}
				}
			}
		} lowestThenLeftmostComp;

		return std::min_element(points.begin(), points.end(), lowestThenLeftmostComp)
			- points.begin();
	}
	
	std::vector<size_t> CH::incremental(const std::vector<Point>& points)
	{
		if (points.size() == 0) return std::vector<size_t>();
		
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
					temp.resize(t - s + 1);
					std::copy(polygon.begin() + s, polygon.begin() + t + 1, temp.begin());
					temp.push_back(x);
				}
				else {
					temp.resize(polygon.size() - (s - t - 1) + 1);
					std::copy(polygon.begin(), polygon.begin() + t + 1, temp.begin());
					temp[t + 1] = x;
					std::copy(polygon.begin() + s, polygon.end(), temp.begin() + t + 2);
				}
				polygon = temp;
			}
		}
		
		return deGroupPoints(dPoints, polygon);
	}
	
	std::vector<size_t> CH::giftWrapping(const std::vector<Point>& points)
	{
		if (points.size() == 0) return std::vector<size_t>();

		auto dPoints = groupPoints(points);
		auto ltl = LTL(dPoints);
		std::vector<size_t> polygon{ ltl };

		struct {
			bool operator()(const DistinctPoint &o,
				const DistinctPoint &p, const DistinctPoint &q) {
				if (o == p) return true;
				if (o == q) return false;
				return q.toRight(o, p);
			}
		} toRightComp;

		while (true)
		{
			auto rightmost = std::max_element(dPoints.begin(), dPoints.end(),
				std::bind(toRightComp, dPoints[polygon.back()], std::placeholders::_1, std::placeholders::_2))
				- dPoints.begin();
						
			if (rightmost == ltl) break;
			else {
				polygon.push_back(rightmost);
			}
		}
		
		return deGroupPoints(dPoints, polygon);
	}
	
	std::vector<size_t> CH::grahamScan(const std::vector<Point>& points)
	{
		if (points.size() == 0) return std::vector<size_t>();

		auto dPoints = groupPoints(points);
		auto ltl = LTL(dPoints);

		struct {
			bool operator()(const DistinctPoint& o,
				const DistinctPoint& p, const DistinctPoint& q) {
				if (o == p) return true;
				if (o == q) return false;
				return p.toRight(o, q);
			}
		} polarComp;

		auto ltlPoint = dPoints[ltl];
		std::sort(dPoints.begin(), dPoints.end(),
			std::bind(polarComp, ltlPoint, std::placeholders::_1, std::placeholders::_2));

		std::vector<size_t> polygon{ 0 };
		if (dPoints.size() == 1) return deGroupPoints(dPoints, polygon);

		polygon.push_back(1);
		for (size_t i = 2; i < dPoints.size(); i++) {
			auto last = polygon.back();
			auto lastpre = *(polygon.end() - 2);

			if (dPoints[i].toRight(dPoints[lastpre], dPoints[last])) {
				do {
					polygon.pop_back();
					if (polygon.size() == 1) break;
					last = polygon.back();
					lastpre = *(polygon.end() - 2);
				} while (dPoints[i].toRight(dPoints[lastpre], dPoints[last]));
				polygon.push_back(i);
			}
			else if (dPoints[i].toLeft(dPoints[lastpre], dPoints[last])) {
				polygon.push_back(i);
			}
		}

		return deGroupPoints(dPoints, polygon);
	}
}