#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "glibby/primitives/point2D.h"

namespace glibby {
  class QuadTreeNode {
    public:
      QuadTreeNode(Point2D* cen, float width, float height, int cap);

      Point2D* center_;
      int capacity_;
      float width_;
      float height_; 
      QuadTreeNode* NW_;
      QuadTreeNode* NE_;
      QuadTreeNode* SW_;
      QuadTreeNode* SE_;
      QuadTreeNode* parent_; 
      Point2D* points_;
  };

  class QuadTree {
    public:
    QuadTree(Point2D* TopL, Point2D* TopR, Point2D* BotL, Point2D* BotR, int capacity = 1);
    QuadTree(Point2D* p, float width, float height, int capacity = 1);
    ~QuadTree();

    private:
      void destroy_sub_tree(QuadTreeNode* ptr);

      QuadTreeNode* ptr_;
      int capacity_;
  };
}

#endif
