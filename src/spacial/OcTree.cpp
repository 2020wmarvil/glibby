#include "glibby/spacial/OcTree.h"
#include "glibby/primitives/point2D.h"
#include "glibby/math/general_math.h"

#include <memory>
#include <cmath>
#include <vector>


namespace glibby 
{
  OcTreeNode::OcTreeNode(std::shared_ptr<Point3> cen, float width, 
      float height, float depth, int cap) 
  {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->divided_ = false;
    this->capacity_ = cap;
  }

  bool OcTreeNode::inside_boundary(Point3* ptr) const 
  {
    if (ptr->points[0] <= this->center_->points[0] + this->width_ / 2 &&
        ptr->points[0] >= this->center_->points[0] - this->width_ / 2 &&
        ptr->points[1] <= this->center_->points[1] + this->height_ / 2 &&
        ptr->points[1] >= this->center_->points[1] - this->height_ / 2 && 
        ptr->points[2] <= this->center_->points[2] + this->depth_ / 2 &&
        ptr->points[2] >= this->center_->points[2] - this->depth_ / 2) 
    {
      return true;
    }
    return false;
  }

  bool OcTreeNode::intersect_boundary(Point3* center,
      float width, float height, float depth) const 
  {
    
    // Left, Right, Top, Bottom, Front, Back of boundary    
    float thisLeft, thisRight, thisTop, thisBottom, thisFront, thisBack; 
    thisLeft = this->center_->points[0] - this->width_ / 2;
    thisRight = this->center_->points[0] + this->width_ / 2;
    thisTop = this->center_->points[1] + this->height_ / 2;
    thisBottom = this->center_->points[1] - this->height_ / 2;
    thisFront = this->center_->points[2] + this->depth_ / 2;
    thisBack = this->center_->points[2] - this->depth_ / 2;

    // for other point
    float otherLeft, otherRight, otherTop, otherBottom, otherFront, otherBack; 
    otherLeft = center->points[0] - width / 2;
    otherRight = center->points[0] + width / 2;
    otherTop = center->points[1] + height / 2;
    otherBottom = center->points[1] - height / 2;
    otherFront = center->points[1] + depth / 2;
    otherBack = center->points[1] - depth / 2;

    if (otherLeft > thisRight || otherRight < thisLeft || 
        otherTop < thisBottom || otherBottom > thisTop ||
        otherBack < thisFront || otherFront > thisBack) 
    {
      return false;
    }
    return true;
  }

  OcTree::OcTree(std::shared_ptr<Point3> p, float width, float height, 
      float depth, int capacity) 
  {
    std::shared_ptr<Point3> temp(new Point3);
    temp->points[0] = p->points[0];
    temp->points[1] = p->points[1];
    temp->points[2] = p->points[2];
    if (capacity > 0) 
    {
      this->capacity_ = capacity;
    } 
    else 
    {
      this->capacity_ = 1;
    }
    this->node_ = std::shared_ptr<OcTreeNode>(
        new OcTreeNode(temp,width,height,depth,this->capacity_));
    this->size_ = 0;
  }

  bool OcTree::insert(Point3* point) 
  {
    // out of bounds
    if (!this->node_->inside_boundary(point)) 
    {
      return false;
    }
    return add_point(this->node_, point);
  }

  bool OcTree::remove(Point3* point) 
  {
    // out of bounds
    if (!this->node_->inside_boundary(point)) 
    {
      return false;
    }
    return remove_point(this->node_, point);
  }

  bool OcTree::contains(Point3* point) const 
  {
    if (!this->node_->inside_boundary(point)) 
    {
      return false;
    }
    return search(this->node_,point);
  }

  std::vector<Point3> OcTree::query (Point3* point, 
      float width, float height, float depth) const 
  {
    std::vector<Point3> points;
    search_tree(&points, this->node_, point, width, height, depth);
    return points;
  }

  void OcTree::clear() {
    this->size_ = 0;
    std::shared_ptr<OcTreeNode> temp;
    temp.reset( new OcTreeNode(
        this->node_->center_,
        this->node_->width_,
        this->node_->height_,
        this->node_->depth_,
        this->node_->capacity_));
    this->node_ = temp;
  }



  bool OcTree::add_point(std::shared_ptr<OcTreeNode> node, Point3* point) 
  {
    // is it inside the boundary of the node?
    if (!node->inside_boundary(point)) 
    {
      return false;
    }
    // if we have space at this node, add it here
    if (node->points_.size() < node->capacity_) 
    {
      node->points_.push_back(std::shared_ptr<Point3>(new Point3));
      node->points_[node->points_.size()-1]->points[0] = point->points[0];
      node->points_[node->points_.size()-1]->points[1] = point->points[1];
      node->points_[node->points_.size()-1]->points[2] = point->points[2];
      this->size_++;
      return true;
    }
    
    // if we need to subdivide this node, do so
    if (!node->divided_) 
    {
      subdivide(node);
    }
    // WEST/east (W/E) of center
    if (point->points[0] < node->center_->points[0]) 
    {
      // north/SOUTH (N/S) of center
      if (point->points[1] < node->center_->points[1]) 
      {
        // FRONT/back (F/B) of center
        if (point->points[2] < node->center_->points[2])
        {
          // SWF
          return add_point(node->SWF_,point);
        }
        else
        {
          //SWB
          return add_point(node->SWB_,point);
        }

      } 
      else 
      {
        // FRONT/back (F/B) of center
        if (point->points[2] < node->center_->points[2])
        {
          // NWF
          return add_point(node->NWF_,point);
        }
        else
        {
          // NWB
          return add_point(node->NWB_,point);
        }

      }
    } 
    else 
    { // E of center
      // north/SOUTH (N/S) of center
      if (point->points[1] < node->center_->points[1]) 
      {
        // FRONT/back (F/B) of center
        if (point->points[2] < node->center_->points[2])
        {
          // SEF
          return add_point(node->SEF_,point);
        }
        else
        {
          //SEB
          return add_point(node->SEB_,point);
        }

      } 
      else 
      {
        // FRONT/back (F/B) of center
        if (point->points[2] < node->center_->points[2])
        {
          // NEF
          return add_point(node->NEF_,point);
        }
        else
        {
          // NEB
          return add_point(node->NEB_,point);
        }

      }
    }
  }

  bool OcTree::remove_point(std::shared_ptr<OcTreeNode> node, 
      Point3* point) 
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
      if (node->points_[i]->x == point->x && node->points_[i]->y == point->y) 
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
    if (point->x < node->center_->x) 
    {
      // above/below (N/S) of center
      if (point->y < node->center_->y) 
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
      if (point->y < node->center_->y) 
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


  void OcTree::subdivide(std::shared_ptr<OcTreeNode> node) 
  {
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
        new OcTreeNode(SW_center,new_width,new_height,node->capacity_)
        );

    SE_center->x = node->center_->x + node->width_ / 4;
    SE_center->y = node->center_->y - node->height_ / 4;
    node->SE_.reset(
        new OcTreeNode(SE_center,new_width,new_height,node->capacity_)
        );

    NW_center->x = node->center_->x - node->width_ / 4;
    NW_center->y = node->center_->y + node->height_ / 4;
    node->NW_.reset(
        new OcTreeNode(NW_center,new_width,new_height,node->capacity_)
        );

    NE_center->x = node->center_->x + node->width_ / 4;
    NE_center->y = node->center_->y + node->height_ / 4;
    node->NE_.reset(
        new OcTreeNode(NE_center,new_width,new_height,node->capacity_)
        );
  }

  bool OcTree::search(std::shared_ptr<OcTreeNode> node,
      Point2D* point) const 
  {
    if (!node->inside_boundary(point)) 
    {
      return false;
    }
    // check all points at this node
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (fabsf(node->points_[i]->x - point->x) < FLT_NEAR_ZERO  &&
          fabsf(node->points_[i]->y - point->y) < FLT_NEAR_ZERO) 
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
    if (point->x < node->center_->x) 
    {
      // is point to N or S
      if (point->y < node->center_->y) 
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
      if (point->y < node->center_->y) 
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

  void OcTree::search_tree(
      std::vector<Point2D>* points, std::shared_ptr<OcTreeNode> node, 
      Point2D* center, float width, float height) const 
  {
    if (!node->intersect_boundary(center, width, height)) 
    {
      return;
    }
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (node->points_[i]->x < center->x + width/2 &&
          node->points_[i]->x > center->x - width/2 &&
          node->points_[i]->y < center->y + height/2 &&
          node->points_[i]->y > center->y - height/2) 
      {
        Point2D temp;
        temp.x = node->points_[i]->x;
        temp.y = node->points_[i]->y;
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
