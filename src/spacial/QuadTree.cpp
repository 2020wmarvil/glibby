#include "glibby/spacial/QuadTree.h"
#include "glibby/primitives/point2D.h"
#include "glibby/math/general_math.h"

#include <memory>
#include <cmath>
#include <vector>


namespace glibby {
  QuadTreeNode::QuadTreeNode(std::shared_ptr<Point2D> cen, float width, 
      float height, int cap) {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->divided_ = false;
    this->capacity_ = cap;
  }

  bool QuadTreeNode::inside_boundary(std::shared_ptr<Point2D> ptr) const {
    if (ptr->x <= this->center_->x + this->width_ / 2 &&
        ptr->x >= this->center_->x - this->width_ / 2 &&
        ptr->y <= this->center_->y + this->height_ / 2 &&
        ptr->y >= this->center_->y - this->height_ / 2) {
      return true;
    }
    return false;
  }

  bool QuadTreeNode::intersect_boundary(std::shared_ptr<Point2D> center,
      float width, float height) const {
    float thisL, thisR, thisT, thisB; // Left, Right, Top, Bottom of boundary
    thisL = this->center_->x - this->width_ / 2;
    thisR = this->center_->x + this->width_ / 2;
    thisT = this->center_->y + this->height_ / 2;
    thisB = this->center_->y - this->height_ / 2;

    // for other point
    float otherL, otherR, otherT, otherB; 
    otherL = center->x - width / 2;
    otherR = center->x + width / 2;
    otherT = center->y + height / 2;
    otherB = center->y - height / 2;

    if (otherL > thisR || otherR < thisL || otherT < thisB || otherB > thisT) {
      return false;
    }
    return true;
  }

  QuadTree::QuadTree(std::shared_ptr<Point2D> TopL, 
      std::shared_ptr<Point2D> TopR, std::shared_ptr<Point2D> BotL, 
      std::shared_ptr<Point2D> BotR, int capacity) {
    if (capacity > 0) {
      this->capacity_ = capacity;
    } else {
      this->capacity_ = 1;
    }
    this->tree_points_ = 0;

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
    if (capacity > 0) {
      this->capacity_ = capacity;
    } else {
      this->capacity_ = 1;
    }
    this->tree_points_ = 0;
  }

  bool QuadTree::insert(std::shared_ptr<Point2D> point) {
    // out of bounds
    if (!this->node_->inside_boundary(point)) {
      return false;
    }
    return add_point(this->node_, point);
  }

  bool QuadTree::contains(std::shared_ptr<Point2D> point) const {
    if (!this->node_->inside_boundary(point)) {
      return false;
    }
    return search(this->node_,point);
  }

  const std::vector<std::shared_ptr<const Point2D>>
    QuadTree::query (std::shared_ptr<Point2D> point, float width, 
    float height) const {
      std::vector<std::shared_ptr<const Point2D>> points;
      search_tree(&points, this->node_, point, width, height);
      return points;
    }



  bool QuadTree::add_point(std::shared_ptr<QuadTreeNode> node,
      std::shared_ptr<Point2D> point) {
    // is it inside the boundary of the node?
    if (!node->inside_boundary(point)) {
      return false;
    }
    // if we have space at this node, add it here
    if (node->points_.size() < node->capacity_) {
      node->points_.push_back(point);
      this->tree_points_++;
      return true;
    }
    
    // if we need to subdivide this node, do so
    if (!node->divided_) {
      subdivide(node);
    }
    // are we to the left (W) of center
    if (point->x < node->center_->x) {
      // above/below (N/S) of center
      if (point->y < node->center_->y) {
        // SW
        return add_point(node->SW_, point);

      } else {
        // NW
        return add_point(node->NW_, point);

      }
    } else { // E of center
      // above/below (N/S) of center
      if (point->y < node->center_->y) {
        // SE
        return add_point(node->SE_, point);

      } else {
        // NE
        return add_point(node->NE_, point);

      }
    }
  }

  void QuadTree::subdivide(std::shared_ptr<QuadTreeNode> node) {
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
    node->NE_.reset(
        new QuadTreeNode(NE_center,new_width,new_height,node->capacity_)
        );
  }

  bool QuadTree::search(std::shared_ptr<QuadTreeNode> node,
      std::shared_ptr<Point2D> point) const {
    if (!node->inside_boundary(point)) {
      return false;
    }
    // check all points at this node
    for (long unsigned i=0; i < node->points_.size(); i++) {
      if (fabsf(node->points_[i]->x - point->x) < FLT_NEAR_ZERO  &&
          fabsf(node->points_[i]->y - point->y) < FLT_NEAR_ZERO) {
        return true;
      } 
    }
    // not at this node, so check sub-node it might be in, but first
    // check if we actually have any sub-nodes
    if (!node->divided_) {
      return false;
    }
    // is point to E or W
    if (point->x < node->center_->x) {
      // is point to N or S
      if (point->y < node->center_->y) {
        // SW
        return search(node->SW_,point);
      } else {
        // NW
        return search(node->NW_,point);
      }
    } else {
      // is point to N or S
      if (point->y < node->center_->y) {
        // SE
        return search(node->SE_,point);
      } else {
        // SW
        return search(node->NE_,point);
      }
    }
  }

  void QuadTree::search_tree(
      std::vector<std::shared_ptr<const Point2D>>* points,
      std::shared_ptr<QuadTreeNode> node, std::shared_ptr<Point2D> center, 
      float width, float height) const {
    if (!node->intersect_boundary(center, width, height)) {
      return;
    }
    for (long unsigned i=0; i < node->points_.size(); i++) {
      if (node->points_[i]->x < center->x + width/2 &&
          node->points_[i]->x > center->x - width/2 &&
          node->points_[i]->y < center->y + height/2 &&
          node->points_[i]->y > center->y - height/2) {
        points->push_back(node->points_[i]);        
      }
    }
    search_tree(points,node->SW_,center,width,height);
    search_tree(points,node->SE_,center,width,height);
    search_tree(points,node->NW_,center,width,height);
    search_tree(points,node->NE_,center,width,height);
  }
  
}
