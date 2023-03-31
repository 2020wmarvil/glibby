#include "glibby/spatial/QuadTree.h"
#include "glibby/primitives/point.h"
#include "glibby/math/general_math.h"

#include <memory>
#include <cmath>
#include <vector>


namespace glibby 
{
  QuadTreeNode::QuadTreeNode(std::shared_ptr<Point2> cen, float width, 
      float height, int cap) 
  {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->divided_ = false;
    this->capacity_ = cap;
  }

  bool QuadTreeNode::inside_boundary(Point2* ptr) const 
  {
    if (ptr->coord[0] <= this->center_->coord[0] + this->width_ / 2 &&
        ptr->coord[0] >= this->center_->coord[0] - this->width_ / 2 &&
        ptr->coord[1] <= this->center_->coord[1] + this->height_ / 2 &&
        ptr->coord[1] >= this->center_->coord[1] - this->height_ / 2)
    {
      return true;
    }
    return false;
  }

  bool QuadTreeNode::intersect_boundary(Point2* center,
      float width, float height) const 
  {
    float thisL, thisR, thisT, thisB; // Left, Right, Top, Bottom of boundary
    thisL = this->center_->coord[0] - this->width_ / 2;
    thisR = this->center_->coord[0] + this->width_ / 2;
    thisT = this->center_->coord[1] + this->height_ / 2;
    thisB = this->center_->coord[1] - this->height_ / 2;

    // for other point
    float otherL, otherR, otherT, otherB; 
    otherL = center->coord[0] - width / 2;
    otherR = center->coord[0] + width / 2;
    otherT = center->coord[1] + height / 2;
    otherB = center->coord[1] - height / 2;

    if (otherL > thisR || otherR < thisL || otherT < thisB || otherB > thisT) 
    {
      return false;
    }
    return true;
  }

  QuadTree::QuadTree(std::shared_ptr<Point2> TopL, 
      std::shared_ptr<Point2> TopR, std::shared_ptr<Point2> BotL, 
      std::shared_ptr<Point2> BotR, int capacity) 
  {
    if (capacity > 0) 
    {
      this->capacity_ = capacity;
    } 
    else 
    {
      this->capacity_ = 1;
    }
    this->size_ = 0;

    // using points to create rectangle that will be the bounding box
    float width = TopR->coord[0] - TopL->coord[0];
    float height = TopR->coord[1] - BotR->coord[1];
    
    std::shared_ptr<Point2> temp(new glibby::Point2);
    temp->coord[0] = BotR->coord[0] + width / 2;
    temp->coord[1] = BotR->coord[1] + height / 2;

    this->node_ = std::shared_ptr<QuadTreeNode>(
        new QuadTreeNode(temp, width, height, capacity));
  }
   
  QuadTree::QuadTree(std::shared_ptr<Point2> p, float width, float height, 
      int capacity) 
  {
    std::shared_ptr<Point2> temp(new Point2);
    temp->coord[0] = p->coord[0];
    temp->coord[1] = p->coord[1];
    this->node_ = std::shared_ptr<QuadTreeNode>(
        new QuadTreeNode(temp,width,height,capacity));
    if (capacity > 0) 
    {
      this->capacity_ = capacity;
    } 
    else 
    {
      this->capacity_ = 1;
    }
    this->size_ = 0;
  }

  bool QuadTree::insert(Point2* point) 
  {
    // out of bounds
    if (!this->node_->inside_boundary(point)) 
    {
      return false;
    }
    return add_point(this->node_, point);
  }

  bool QuadTree::remove(Point2* point) 
  {
    // out of bounds
    if (!this->node_->inside_boundary(point)) 
    {
      return false;
    }
    return remove_point(this->node_, point);
  }

  bool QuadTree::contains(Point2* point) const 
  {
    if (!this->node_->inside_boundary(point)) 
    {
      return false;
    }
    return search(this->node_,point);
  }

  std::vector<Point2> QuadTree::query (Point2* point, 
      float width, float height) const 
  {
    std::vector<Point2> points;
    search_tree(&points, this->node_, point, width, height);
    return points;
  }

  void QuadTree::clear() {
    this->size_ = 0;
    std::shared_ptr<QuadTreeNode> temp;
    temp.reset( new QuadTreeNode(
        this->node_->center_,
        this->node_->width_,
        this->node_->height_,
        this->node_->capacity_));
    this->node_ = temp;
  }



  bool QuadTree::add_point(std::shared_ptr<QuadTreeNode> node, Point2* point) 
  {
    // is it inside the boundary of the node?
    if (!node->inside_boundary(point)) 
    {
      return false;
    }
    // if we have space at this node, add it here
    if (node->points_.size() < node->capacity_) 
    {
      node->points_.push_back(std::shared_ptr<Point2>(new Point2));
      node->points_[node->points_.size()-1]->coord[0] = point->coord[0];
      node->points_[node->points_.size()-1]->coord[1] = point->coord[1];
      this->size_++;
      return true;
    }
    
    // if we need to subdivide this node, do so
    if (!node->divided_) 
    {
      subdivide(node);
    }
    // are we to the left (W) of center
    if (point->coord[0] < node->center_->coord[0])
    {
      // above/below (N/S) of center
      if (point->coord[1] < node->center_->coord[1])
      {
        // SW
        return add_point(node->SW_, point);

      } 
      else 
      {
        // NW
        return add_point(node->NW_, point);

      }
    } 
    else 
    { // E of center
      // above/below (N/S) of center
      if (point->coord[1] < node->center_->coord[1])
      {
        // SE
        return add_point(node->SE_, point);

      } 
      else 
      {
        // NE
        return add_point(node->NE_, point);

      }
    }
  }

  bool QuadTree::remove_point(std::shared_ptr<QuadTreeNode> node, 
      Point2* point) 
  {
    // is it inside the boundary of the node?
    if (!node->inside_boundary(point)) 
    {
      return false;
    }
    // if point is here, remove it
    for (unsigned long i=0; i < node->points_.size(); i++) 
    {
      // check if points are the same
      if (node->points_[i]->coord[0] == point->coord[0] && node->points_[i]->coord[1] == point->coord[1])
      {
        node->points_.erase(node->points_.begin() + i);
        return true;
      }
    }
    
    // if no sub-nodes, return false
    if (!node->divided_) 
    {
      return false;
    }
    // are we to the left (W) of center
    if (point->coord[0] < node->center_->coord[0])
    {
      // above/below (N/S) of center
      if (point->coord[1] < node->center_->coord[1])
      {
        // SW
        return remove_point(node->SW_, point);
      } 
      else 
      {
        // NW
        return remove_point(node->NW_, point);
      }
    } 
    else 
    { // E of center
      // above/below (N/S) of center
      if (point->coord[1] < node->center_->coord[1])
      {
        // SE
        return remove_point(node->SE_, point);
      } 
      else 
      {
        // NE
        return remove_point(node->NE_, point);
      }
    }
  }


  void QuadTree::subdivide(std::shared_ptr<QuadTreeNode> node) 
  {
    node->divided_ = true;

    float new_width = node->width_ / 2;
    float new_height = node->height_ / 2;
    
    std::shared_ptr<Point2> SW_center = std::make_shared<Point2>();
    std::shared_ptr<Point2> SE_center = std::make_shared<Point2>();
    std::shared_ptr<Point2> NW_center = std::make_shared<Point2>();
    std::shared_ptr<Point2> NE_center = std::make_shared<Point2>();

    SW_center->coord[0] = node->center_->coord[0] - node->width_ / 4;
    SW_center->coord[1] = node->center_->coord[1] - node->height_ / 4;
    node->SW_.reset(
        new QuadTreeNode(SW_center,new_width,new_height,node->capacity_)
        );

    SE_center->coord[0] = node->center_->coord[0] + node->width_ / 4;
    SE_center->coord[1] = node->center_->coord[1] - node->height_ / 4;
    node->SE_.reset(
        new QuadTreeNode(SE_center,new_width,new_height,node->capacity_)
        );

    NW_center->coord[0] = node->center_->coord[0] - node->width_ / 4;
    NW_center->coord[1] = node->center_->coord[1] + node->height_ / 4;
    node->NW_.reset(
        new QuadTreeNode(NW_center,new_width,new_height,node->capacity_)
        );

    NE_center->coord[0] = node->center_->coord[0] + node->width_ / 4;
    NE_center->coord[1] = node->center_->coord[1] + node->height_ / 4;
    node->NE_.reset(
        new QuadTreeNode(NE_center,new_width,new_height,node->capacity_)
        );
  }

  bool QuadTree::search(std::shared_ptr<QuadTreeNode> node,
      Point2* point) const 
  {
    if (!node->inside_boundary(point)) 
    {
      return false;
    }
    // check all points at this node
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (fabsf(node->points_[i]->coord[0] - point->coord[0]) < FLT_NEAR_ZERO &&
          fabsf(node->points_[i]->coord[1] - point->coord[1]) < FLT_NEAR_ZERO)
      {
        return true;
      } 
    }
    // not at this node, so check sub-node it might be in, but first
    // check if we actually have any sub-nodes
    if (!node->divided_) 
    {
      return false;
    }
    // is point to E or W
    if (point->coord[0] < node->center_->coord[0])
    {
      // is point to N or S
      if (point->coord[1] < node->center_->coord[1])
      {
        // SW
        return search(node->SW_,point);
      } 
      else 
      {
        // NW
        return search(node->NW_,point);
      }
    } 
    else 
    {
      // is point to N or S
      if (point->coord[1] < node->center_->coord[1])
      {
        // SE
        return search(node->SE_,point);
      } 
      else 
      {
        // SW
        return search(node->NE_,point);
      }
    }
  }

  void QuadTree::search_tree(
      std::vector<Point2>* points, std::shared_ptr<QuadTreeNode> node, 
      Point2* center, float width, float height) const 
  {
    if (!node->intersect_boundary(center, width, height)) 
    {
      return;
    }
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (node->points_[i]->coord[0] < center->coord[0] + width / 2 &&
          node->points_[i]->coord[0] > center->coord[0] - width / 2 &&
          node->points_[i]->coord[1] < center->coord[1] + height / 2 &&
          node->points_[i]->coord[1] > center->coord[1] - height / 2)
      {
        Point2 temp;
        temp.coord[0] = node->points_[i]->coord[0];
        temp.coord[1] = node->points_[i]->coord[1];
        points->push_back(temp);        
      }
    }
    if (!node->divided_) 
    {
      return;
    }
    search_tree(points,node->SW_,center,width,height);
    search_tree(points,node->SE_,center,width,height);
    search_tree(points,node->NW_,center,width,height);
    search_tree(points,node->NE_,center,width,height);
  }
  
}
