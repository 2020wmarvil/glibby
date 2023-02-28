#pragma once

#include <vector>
#include <map>
#include <queue>
#include "glibby/primitives/point.h"
#include <iostream>
#include <string>

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
	class kdTree {
	private:
		std::vector<glibby::Point<T, N>> points;
		Node<glibby::Point<T, N>>* parent = NULL;

		bool find(Node<glibby::Point<T, N>>*& node, glibby::Point<T, N> point) {
			if (!node) {
				return false;
			}

			if (node->val == point) {
				return true;
			}
			return find(node->left, point) || find(node->right, point);
		}

		void construct_tree(std::vector<glibby::Point<T, N>>& points, Node<glibby::Point<T, N>>*& node, int depth) {
			if (points.size() == 0) {
				return;
			}

			if (points.size() == 1) {
				node = new Node(points[0]);
				return;
			}

			std::vector<Point<T, N>> left;
			std::vector<Point<T, N>> right;

			std::map<int, std::vector<Point<T, N>>> mp;
			for (int i = 0; i < points.size(); i++) {
				mp[points[i].coord[depth]].push_back(points[i]);
			}

			int half = points.size() / 2;
			int index = 0;
			for (auto x : mp) {
				for (int i = 0; i < x.second.size(); i++) {
					if (index < half) {
						left.push_back(x.second[i]);
					}
					else if (index > half) {
						right.push_back(x.second[i]);
					}
					else {
						node = new Node<glibby::Point<T, N>>(x.second[i]);
					}
					index++;
				}
			}
			construct_tree(left, node->left, (depth + 1) % N);
			construct_tree(right, node->right, (depth + 1) % N);
		}

		void destroy_tree(Node<glibby::Point<T, N>>* node) {
			if (node->left) {
				destroy_tree(node->left);
			}
			if (node->right) {
				destroy_tree(node->right);
			}
			delete(node);
		}

	public:
		kdTree(std::vector<glibby::Point<T, N>> pointss) {
			for (int i = 0; i < pointss.size(); i++) {
				points.push_back(pointss[i]);
			}
			construct_tree(pointss, parent, 0);
		}

		kdTree(Point<T, N> point) {
			points.push_back(point);
			parent = new Node<glibby::Point<T, N>>(point);
		}

		~kdTree() { destroy_tree(parent); }


		void insert(glibby::Point<T, N> point) {
			if (find(parent, point)) {
				return;
			}

			destroy_tree(parent);
			points.push_back(point);
			construct_tree(points, parent, 0);
		}

		void insert(std::vector<glibby::Point<T, N>>& point) {
			for (int i = 0; i < point.size(); i++) {
				insert(point[i]);
			}
		}

		void remove(glibby::Point<T, N> point) {
			if (!find(parent, point)) {
				return;
			}

			destroy_tree(parent);
			std::vector<glibby::Point<T, N>> temp;
			for (int i = 0; i < points.size(); i++) {
				if (points[i] != point) {
					temp.push_back(points[i]);
				}
			}
			points.clear();
			for (int i = 0; i < temp.size(); i++) {
				points.push_back(temp[i]);
			}
			construct_tree(points, parent, 0);
		}

		void remove(std::vector<glibby::Point<T, N>>& point) {
			for (int i = 0; i < point.size(); i++) {
				remove(point[i]);
			}
		}

		

		//Help for debugging(may or may not be in the final version)
		void BFSprint() {
			if (!parent) {
				return;
			}
			glibby::Point<T, N> p = parent->val;
			std::queue<Node<glibby::Point<T, N>>*> q;
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
					if(f->left){
						q.push(f->left);
					}
					if (f->right) {
						q.push(f->right);
					}
				}
				layer++;
			}
		}
	};
}
