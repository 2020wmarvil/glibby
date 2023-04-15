#pragma once

#include <cmath>
#include <iostream>
#include <vector>

namespace glibby
{

	//The Point class takes in a list of values at each of the different axis
	//The Point class also contains the number of different axis
	template<typename T, int N>
	struct Point
	{
		int axis = N;
		T coord[N];

		//There are four constructors in which the point has no input or takes in a list of values
		Point() { for (size_t i = 0; i < N; i++) { coord[i] = 0; } };
		explicit Point(T all) { for (size_t i = 0; i < N; i++) coord[i] = all; }
		explicit Point(T arr[N]) { for (size_t i = 0; i < N; i++) coord[i] = arr[i]; }
		explicit Point(std::vector<T> arr) { for (size_t i = 0; i < N; i++) coord[i] = arr[i]; }


		//The function checks to see if two points have the same values at the same axis
		bool operator==(const Point<T, N> p) 
		{
			for (int i = 0; i < N; i++) {
				if (coord[i] != p.coord[i]) {
					return false;
				}
			}
			return true;
		}
	};

	//The definitions below are defined for quicker use in other files
	typedef Point<float, 1> Point1;
	typedef Point<float, 2> Point2;
	typedef Point<float, 3> Point3;
	typedef Point<double, 1> Point1d;
	typedef Point<double, 2> Point2d;
	typedef Point<double, 3> Point3d;
	typedef Point<uint32_t, 1> Point1i;
	typedef Point<uint32_t, 2> Point2i;
	typedef Point<uint32_t, 3> Point3i;

	//The ManhattanDistance takes two points and calculates its manhattan distance
	template<typename T, int N>
	T ManhattanDistance(const Point<T, N>& p1, const Point<T, N>& p2) 
	{
		T total = 0;
		for (int i = 0; i < p1.axis; i++) {
			total += abs(p1.coord[i] - p2.coord[i]);
		}
		return total;
	}

	//The EuclideanDistance takes two points and calculates its Euclidean distance
	template<typename T, int N>
	T EuclideanDistance(const Point<T, N>& p1, const Point<T, N>& p2) 
	{
		T total = 0;
		for (int i = 0; i < p1.axis; i++) {
			total += ((p1.coord[i] - p2.coord[i]) * (p1.coord[i] - p2.coord[i]));
		}
		return std::sqrt(total);
	}
}
