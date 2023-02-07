#include "glibby/spacial/QuadTree.h"
#include "glibby/primitives/point2D.h"

#include <memory>

namespace glibby {
  QuadTreeNode::QuadTreeNode(std::shared_ptr<Point2D> cen, float width, 
      float height, int cap) {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->divided_ = false;
    this->capacity_ = cap;
    this->num_points_ = 0;
    this->points_.reset(new std::shared_ptr<Point2D>[cap]);
  }

  QuadTree::QuadTree(std::shared_ptr<Point2D> TopL, 
      std::shared_ptr<Point2D> TopR, std::shared_ptr<Point2D> BotL, 
      std::shared_ptr<Point2D> BotR, int capacity) {
    this->capacity_ = capacity;

    // using points to create rectangle that will be the bounding box
    float width = TopR->x - TopL->x;
    float height = TopR->y - BotR->y;
    
    std::shared_ptr<Point2D> temp(new Point2D);
    temp->x = BotR->x + width/2;
    temp->y = BotR->y + height/2;

    this->node_ = std::shared_ptr<QuadTreeNode>(
        new QuadTreeNode(temp, width, height, capacity));
  }
   
  QuadTree::QuadTree(std::shared_ptr<Point2D> p, float width, float height, 
      int capacity) {
    this->node_ = std::shared_ptr<QuadTreeNode>(
        new QuadTreeNode(p,width,height,capacity));
    this->capacity_ = capacity;
  }

  bool QuadTree::insert(std::shared_ptr<Point2D> point) {
    // out of bounds
    if (point->x > this->node_->center_->x + this->node_->width_ / 2 ||
        point->x < this->node_->center_->x - this->node_->width_ / 2 ||
        point->y > this->node_->center_->y + this->node_->height_ / 2 ||
        point->y < this->node_->center_->y - this->node_->height_ / 2) {
      return false;
    }
    return add_point(this->node_, point);
  }



  bool QuadTree::add_point(std::shared_ptr<QuadTreeNode> node,
      std::shared_ptr<Point2D> point) {
    if (node->num_points_ < node->capacity_) {
      node->points_[node->num_points_] = point;
      node->num_points_++;
      return true;
    }
    
    if (!node->divided_) {
      subdivide(node);
    }
    if (point->x < node->center_->x) {
      if (point->y < node->center_->y) {
        // SW
        return add_point(node->SW_, point);

      } else {
        // SE
        return add_point(node->SE_, point);

      }
    } else {
      if (point->y < node->center_->y) {
        // NW
        return add_point(node->NW_, point);

      } else {
        // NE
        return add_point(node->NE_, point);

      }
    }
  }

  void subdivide(std::shared_ptr<QuadTreeNode> node) {
    node->divided_ = true;

    float new_width = node->width_ / 2;
    float new_height = node->height_ / 2;
    
    std::shared_ptr<Point2D> SW_center = std::make_shared<Point2D>();
    std::shared_ptr<Point2D> SE_center = std::make_shared<Point2D>();
    std::shared_ptr<Point2D> NW_center = std::make_shared<Point2D>();
    std::shared_ptr<Point2D> NE_center = std::make_shared<Point2D>();

    SW_center->x = node->center_->x - node->width_ / 4;
    SW_center->y = node->center_->y - node->height_ / 4;
    node->SW_.reset(
        new QuadTreeNode(SW_center,new_width,new_height,node->capacity_)
        );

    SE_center->x = node->center_->x + node->width_ / 4;
    SE_center->y = node->center_->y - node->height_ / 4;
    node->SE_.reset(
        new QuadTreeNode(SE_center,new_width,new_height,node->capacity_)
        );

    NW_center->x = node->center_->x - node->width_ / 4;
    NW_center->y = node->center_->y + node->height_ / 4;
    node->NW_.reset(
        new QuadTreeNode(NW_center,new_width,new_height,node->capacity_)
        );

    NE_center->x = node->center_->x + node->width_ / 4;
    NE_center->y = node->center_->y + node->height_ / 4;
    node->SE_.reset(
        new QuadTreeNode(NE_center,new_width,new_height,node->capacity_)
        );
  }
}
