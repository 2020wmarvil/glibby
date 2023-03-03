#pragma once

#include "glibby/primitives/point.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

namespace glibby
{
	template<typename T>
	struct Node
	{
		T val;
		Node<T>* left;
		Node<T>* right;

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
			parent = new Node<Point<T, N>>(point);
		}

		~kdTree() { DestroyTree(parent); }


		void Insert(const Point<T, N>& point) {
			if (Find(parent, point)) {
				return;
			}

			DestroyTree(parent);
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

			DestroyTree(parent);
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
		void BFSprint() {
			if (!parent) {
				return;
			}
			Point<T, N> p = parent->val;
			std::queue<Node<Point<T, N>>*> q;
			q.push(parent);
			int layer = 0;
			while (q.size() > 0) {
				std::cout << "Layer " << layer << std::endl;
				int w = q.size();
				for (int i = 0; i < w; i++) {
					Node<glibby::Point<T, N>>* f = q.front();
					q.pop();
					std::cout << "[";
					for (int j = 0; j < f->val.axis; j++) {
						std::cout << " " << f->val.coord[j] << " ";
					}
					std::cout << "]" << std::endl;
					if (f->left) {
						q.push(f->left);
					}
					if (f->right) {
						q.push(f->right);
					}
				}
				layer++;
			}
		}

	private:
		std::vector<Point<T, N>> points;
		Node<Point<T, N>>* parent = NULL;

		bool Find(Node<Point<T, N>>*& node, const Point<T, N>& point) {
			if (!node) {
				return false;
			}

			if (node->val == point) {
				return true;
			}

			return Find(node->left, point) || Find(node->right, point);
		}

		void ConstructTree(const std::vector<Point<T, N>>& points, Node<Point<T, N>>*& node, int depth) {
			if (points.size() == 0) {
				return;
			}

			if (points.size() == 1) {
				node = new Node(points[0]);
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
						node = new Node<Point<T, N>>(x.second[i]);
					}
					index++;
				}
			}

			ConstructTree(left, node->left, (depth + 1) % N);
			ConstructTree(right, node->right, (depth + 1) % N);
		}

		void DestroyTree(Node<Point<T, N>>* node) {
			if (node->left) {
				DestroyTree(node->left);
			}
			if (node->right) {
				DestroyTree(node->right);
			}

			delete(node);
		}
	};
}
