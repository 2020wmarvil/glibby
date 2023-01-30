#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "glibby/primitives/point2D.h"

namespace glibby {
  class QuadTree {
    public:
    QuadTree(Point2D& p1, Point2D& p2, Point2D& p3, Point2D& p4, int capacity = 1);
    QuadTree(Point2D* points, int capacity = 1);
    QuadTree(Point2D& p, float width, float height, int capacity = 1);
    
    private:
  };
}

#endif