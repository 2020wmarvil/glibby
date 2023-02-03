#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "glibby/primitives/point2D.h"

#include <memory>

namespace glibby {
  class QuadTreeNode {
    public:
      QuadTreeNode(std::shared_ptr<Point2D> cen, float width, float height, int cap);

      std::shared_ptr<Point2D> center_;
      int capacity_;
      int num_points_;
      float width_;
      float height_; 
      std::shared_ptr<QuadTreeNode> NW_;
      std::shared_ptr<QuadTreeNode> NE_;
      std::shared_ptr<QuadTreeNode> SW_;
      std::shared_ptr<QuadTreeNode> SE_;
      std::shared_ptr<QuadTreeNode> parent_; 
      std::shared_ptr<Point2D> points_;
  };

  class QuadTree {
    public:
      QuadTree(std::shared_ptr<Point2D> TopL, std::shared_ptr<Point2D> TopR, std::shared_ptr<Point2D> BotL, std::shared_ptr<Point2D> BotR, int capacity = 1);
      QuadTree(std::shared_ptr<Point2D> p, float width, float height, int capacity = 1);

    private:
      std::shared_ptr<QuadTreeNode> ptr_;
      int capacity_;
  };
}

#endif
