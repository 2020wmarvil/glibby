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

		friend std::ostream& operator<<(std::ostream& out, const Point<T, N>& point) {
			out << "(";
			for (int i = 0; i < N; i++) {
				out << point.coord[i];
				if (i < N - 1) {
					out << ", ";
				}
			}
			out << ")";
			return out;
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

	template<typename T>
	int direction(Point<T, 2> p, Point<T, 2> q, Point<T, 2> r)
	{
		return (q.coord[1] - p.coord[1]) * (r.coord[0] - q.coord[0]) - (q.coord[0] - p.coord[0]) * (r.coord[1] - q.coord[1]);
	}

	template<typename T>
	float distance(const Point<T, 2>& p1, const Point<T, 2>& p2)
	{
		float deltaX = p1.coord[0] - p2.coord[0];
		float deltaY = p1.coord[1] - p2.coord[1];
		return std::sqrt(deltaX * deltaX + deltaY * deltaY);
	}
}
