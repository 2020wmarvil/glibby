#include "glibby/spacial-algorithms/QuadTree.h"
#include "glibby/primitives/point2D.h"

#include <memory>

namespace glibby {
  QuadTreeNode::QuadTreeNode(std::shared_ptr<Point2D> cen, float width, float height, int cap) {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->capacity_ = cap;
    this->num_points_ = 0;
  }

  QuadTree::QuadTree(std::shared_ptr<Point2D> TopL, std::shared_ptr<Point2D> TopR, std::shared_ptr<Point2D> BotL, std::shared_ptr<Point2D> BotR, int capacity) {
    this->capacity_ = capacity;

    // using points to create rectangle that will be the bounding box
    float width = TopR->x - TopL->x;
    float height = TopR->y - BotR->y;
    
    std::shared_ptr<Point2D> temp( new Point2D);
    temp->x = BotR->x + width/2;
    temp->y = BotR->y + height/2;

    this->ptr_ = std::shared_ptr<QuadTreeNode>(new QuadTreeNode(temp, width, height, capacity));
  }
   
  QuadTree::QuadTree(std::shared_ptr<Point2D> p, float width, float height, int capacity) {
    this->ptr_ = std::shared_ptr<QuadTreeNode>(new QuadTreeNode(p,width,height,capacity));
    this->capacity_ = capacity;
  }
}
