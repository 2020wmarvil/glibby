#pragma once

#include "glibby/primitives/point.h"
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace glibby
{
	//The struct Node is used for the creation of the KD Tree data structure
	//It will contain a point value and two children
	template<typename T>
	struct Node
	{
		T val;
		std::unique_ptr<Node<T>> left;
		std::unique_ptr<Node<T>> right;

		Node() { left = NULL; right = NULL; }
		Node(T newVal) { val = newVal; left = NULL; right = NULL; }
	};

	//The kdTree class contains the data structure of the KD Tree along with algorothmic functions the KD Tree can be used to solve
	template<typename T, int N>
	class kdTree 
	{


	public:

		//The kdTree constructor takes in a list of points and constructs the tree with the given points
		kdTree(std::vector<Point<T, N>> const& NewPoints) {
			for (int i = 0; i < NewPoints.size(); i++) {
				points.push_back(NewPoints[i]);
			}

			ConstructTree(NewPoints, parent, 0);
		}

		//The kdTree constructor takes a singular point and creates a node using the point
		kdTree(Point<T, N> point) {
			points.push_back(point);
			parent = std::make_unique<Node<Point<T, N>>>(point);
		}

		//The Insert function takes in a point and uses the Find function to determine if the point is contained in the tree
		//If the point is in the tree, then we don't add the point into the tree
		//Otherwise, the point is added into the tree
		void Insert(const Point<T, N>& point) {
			if (Find(parent, point)) {
				return;
			}

			points.push_back(point);
			ConstructTree(points, parent, 0);
		}

		//The Insert function takes in a list of points and simply uses the Insert function that takes in one singular point 
		//We repeat the process for all points in the given list
		void Insert(const std::vector<Point<T, N>>& point) {
			for (int i = 0; i < point.size(); i++) {
				Insert(point[i]);
			}
		}

		//The Delete function takes in a point and uses the Find function to determine if the point is contained in the tree
		//If the point is not in the tree, then we don't delete the point from the tree
		//Otherwise, the point is first removed from the list of points and then the tree is reconstructed
		void Delete(const Point<T, N>& point) {
			if (!Find(parent, point)) {
				return;
			}
			std::vector<Point<T, N>> temp;
			for (int i = 0; i < points.size(); i++) {
				if (points[i] != point) {
					temp.push_back(points[i]);
				}
			}
			points.clear();
			for (int i = 0; i < temp.size(); i++) {
				points.push_back(temp[i]);
			}
			ConstructTree(points, parent, 0);
		}

		//The Delete function takes in a list of points and simply uses the Delte function that takes in one singular point 
		//We repeat the process for all points in the given list
		void Delete(const std::vector<Point<T, N>>& points) {
			for (int i = 0; i < points.size(); i++) {
				Delete(points[i]);
			}
		}


		//Help for debugging(will remove when completed with test cases)
		void DFSPrint() {
			DFSPrint(parent);
			std::cout << std::endl;
		}



	private:
		//The points contained in the tree will be stored inside of a vector of points
		std::vector<Point<T, N>> points;
		std::unique_ptr<Node<Point<T, N>>> parent;

		//The Find function takes in a point and goes through the tree, node by node, to determine whether the point exist in the tree
		bool Find(const std::unique_ptr<Node<Point<T, N>>>& node, const Point<T, N>& point) {
			if (!node) {
				return false;
			}

			if (node->val == point) {
				return true;
			}

			return Find(node->left, point) || Find(node->right, point);
		}

		//The ConstrcutTree function takes in a list of points, a unique_pointer that represents the parent node of the tree, and an int depth.
		//We will sort each points by the current axis given by the depth and take the middle point.
		//Any point lower than the middle point will be put into a left list while any point greater than the middle point will be put into a right list
		//The middle point will be used to create a node for the unique_pointer
		//We finally recursively call the ConstructTree function with the left and right list as input.
		void ConstructTree(const std::vector<Point<T, N>>& points, std::unique_ptr<Node<Point<T, N>>>& node, int depth) {
			if (points.size() == 0) {
				return;
			}

			if (points.size() == 1) {
				node = std::make_unique<Node<Point<T, N>>> (points[0]);
				return;
			}

			std::vector<Point<T, N>> left;
			std::vector<Point<T, N>> right;

			std::map<int, std::vector<Point<T, N>>> sortedPointsByAxis;
			for (int i = 0; i < points.size(); i++) {
				sortedPointsByAxis[points[i].coord[depth]].push_back(points[i]);
			}

			int half = points.size() / 2;
			int index = 0;
			for (auto const& x : sortedPointsByAxis) {
				for (int i = 0; i < x.second.size(); i++) {
					if (index < half) {
						left.push_back(x.second[i]);
					}
					else if (index > half) {
						right.push_back(x.second[i]);
					}
					else {
						node = std::make_unique<Node<Point<T, N>>>(x.second[i]);
					}
					index++;
				}
			}

			ConstructTree(left, node->left, (depth + 1) % N);
			ConstructTree(right, node->right, (depth + 1) % N);
		}

		//currently used for debugging purposes. Will remove when test cases are completed
		void DFSPrint(const std::unique_ptr<Node<Point<T, N>>>& node) {
			if (!node) {
				return;
			}

			std::cout << "(";
			for (int i = 0; i < node->val.axis; i++) {
				std::cout << node->val.coord[i];
				if (i < node->val.axis-1) {
					std::cout << ",";
				}
			}
			std::cout << ")" << std::endl;

			DFSPrint(node->left);
			DFSPrint(node->right);
		}
	};
}
