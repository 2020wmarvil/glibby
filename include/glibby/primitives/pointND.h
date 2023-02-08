#pragma once

#include <cmath>

namespace glibby
{
	template<typename T, int N>
	struct PointND
	{
		int axis = N;
		T points[N];
	};

	template<typename T, int N>
	T ManhattanDistance(const PointND<T, N>& p1, const PointND<T, N>& p2) {
		T total = 0;
		for (int i = 0; i < p1.axis; i++) {
			total += abs(p1.points[i] - p2.points[i]);
		}
		return total;
	}

	template<typename T, int N>
	T EuclideanDistance(const PointND<T, N>& p1, const PointND<T, N>& p2) {
		T total = 0;
		for (int i = 0; i < p1.axis; i++) {
			total += ((p1.points[i] - p2.points[i]) * (p1.points[i] - p2.points[i]));
		}
		return std::sqrt(total);
	}


}
