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
	template<typename T>
	struct Node
	{
		T val;
		std::unique_ptr<Node<T>> left;
		std::unique_ptr<Node<T>> right;

		Node() { left = NULL; right = NULL; }
		Node(T newVal) { val = newVal; left = NULL; right = NULL; }
	};

	template<typename T, int N>
	class kdTree 
	{

	public:
		kdTree(std::vector<Point<T, N>> const& NewPoints) {
			for (int i = 0; i < NewPoints.size(); i++) {
				points.push_back(NewPoints[i]);
			}

			ConstructTree(NewPoints, parent, 0);
		}

		kdTree(Point<T, N> point) {
			points.push_back(point);
			parent = std::make_unique<Node<Point<T, N>>>(point);
		}

		void Insert(const Point<T, N>& point) {
			if (Find(parent, point)) {
				return;
			}

			points.push_back(point);
			ConstructTree(points, parent, 0);
		}

		void Insert(const std::vector<Point<T, N>>& point) {
			for (int i = 0; i < point.size(); i++) {
				Insert(point[i]);
			}
		}

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

		void Delete(const std::vector<Point<T, N>>& points) {
			for (int i = 0; i < points.size(); i++) {
				Delete(points[i]);
			}
		}


		//Help for debugging(may or may not be in the final version)
		void DFSPrint() {
			DFSPrint(parent);
			std::cout << std::endl;
		}

	private:
		std::vector<Point<T, N>> points;
		std::unique_ptr<Node<Point<T, N>>> parent;

		bool Find(const std::unique_ptr<Node<Point<T, N>>>& node, const Point<T, N>& point) {
			if (!node) {
				return false;
			}

			if (node->val == point) {
				return true;
			}

			return Find(node->left, point) || Find(node->right, point);
		}

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
