#pragma once

#include <cmath>
#include <vector>

namespace glibby
{
	template<typename T, int N>
	struct Point
	{
		int axis = N;
		T points[N];

		Point() { for (size_t i = 0; i < N; i++) { points[i] = 0; } };
		explicit Point(T all) { for (size_t i = 0; i < N; i++) points[i] = all; }
		explicit Point(T arr[N]) { for (size_t i = 0; i < N; i++) points[i] = arr[i]; }
		explicit Point(std::vector<T> arr) { for (size_t i = 0; i < N; i++) points[i] = arr[i]; }
	};

	typedef Point<float, 1> Point1;
	typedef Point<float, 2> Point2;
	typedef Point<float, 3> Point3;
	typedef Point<double, 1> Point1d;
	typedef Point<double, 2> Point2d;
	typedef Point<double, 3> Point3d;
	typedef Point<uint32_t, 1> Point1i;
	typedef Point<uint32_t, 2> Point2i;
	typedef Point<uint32_t, 3> Point3i;

	template<typename T, int N>
	T ManhattanDistance(const Point<T, N>& p1, const Point<T, N>& p2) 
	{
		T total = 0;
		for (int i = 0; i < p1.axis; i++) {
			total += abs(p1.points[i] - p2.points[i]);
		}
		return total;
	}

	template<typename T, int N>
	T EuclideanDistance(const Point<T, N>& p1, const Point<T, N>& p2) 
	{
		T total = 0;
		for (int i = 0; i < p1.axis; i++) {
			total += ((p1.points[i] - p2.points[i]) * (p1.points[i] - p2.points[i]));
		}
		return std::sqrt(total);
	}
}
