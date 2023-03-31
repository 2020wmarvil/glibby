#include "glibby/spacial/OcTree.h"
#include "glibby/primitives/point.h"
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
    this->depth_ = depth;
    this->divided_ = false;
    this->capacity_ = cap;
  }

  bool OcTreeNode::inside_boundary(Point3* ptr) const 
  {
    if (ptr->coord[0] <= this->center_->coord[0] + this->width_ / 2 &&
        ptr->coord[0] >= this->center_->coord[0] - this->width_ / 2 &&
        ptr->coord[1] <= this->center_->coord[1] + this->height_ / 2 &&
        ptr->coord[1] >= this->center_->coord[1] - this->height_ / 2 && 
        ptr->coord[2] <= this->center_->coord[2] + this->depth_ / 2 &&
        ptr->coord[2] >= this->center_->coord[2] - this->depth_ / 2) 
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
    thisLeft = this->center_->coord[0] - this->width_ / 2;
    thisRight = this->center_->coord[0] + this->width_ / 2;
    thisTop = this->center_->coord[1] + this->height_ / 2;
    thisBottom = this->center_->coord[1] - this->height_ / 2;
    thisFront = this->center_->coord[2] + this->depth_ / 2;
    thisBack = this->center_->coord[2] - this->depth_ / 2;

    // for other point
    float otherLeft, otherRight, otherTop, otherBottom, otherFront, otherBack; 
    otherLeft = center->coord[0] - width / 2;
    otherRight = center->coord[0] + width / 2;
    otherTop = center->coord[1] + height / 2;
    otherBottom = center->coord[1] - height / 2;
    otherFront = center->coord[1] + depth / 2;
    otherBack = center->coord[1] - depth / 2;

    if (thisLeft > otherRight || otherLeft > thisRight ||
        thisBottom > otherTop || otherBottom > thisTop ||
        thisBack > otherFront || otherBack > thisFront)
    {
      return false;
    }
    return true;
  }

  OcTree::OcTree(std::shared_ptr<Point3> p, float width, float height, 
      float depth, int capacity) 
  {
    std::shared_ptr<Point3> temp(new Point3);
    temp->coord[0] = p->coord[0];
    temp->coord[1] = p->coord[1];
    temp->coord[2] = p->coord[2];
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
      node->points_[node->points_.size()-1]->coord[0] = point->coord[0];
      node->points_[node->points_.size()-1]->coord[1] = point->coord[1];
      node->points_[node->points_.size()-1]->coord[2] = point->coord[2];
      this->size_++;
      return true;
    }
    
    // if we need to subdivide this node, do so
    if (!node->divided_) 
    {
      subdivide(node);
    }

    // Add to everything, one of them will be righta
    if (add_point(node->SWF_,point))
    {
      return true;
    }
    else if (add_point(node->SWB_,point))
    {
      return true;
    }
    else if (add_point(node->SEF_,point))
    {
      return true;
    }
    else if (add_point(node->SEB_,point))
    {
      return true;
    }
    else if (add_point(node->NWF_,point))
    {
      return true;
    }
    else if (add_point(node->NWB_,point))
    {
      return true;
    }
    else if (add_point(node->NEF_,point))
    {
      return true;
    }
    else if (add_point(node->NEB_,point))
    {
      return true;
    }
    return false;
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
      if (node->points_[i]->coord[0] == point->coord[0] && 
          node->points_[i]->coord[1] == point->coord[1] &&
          node->points_[i]->coord[2] == point->coord[2]) 
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
 
    // Remove from everything, one of them will be righta
    if (remove_point(node->SWF_,point))
    {
      return true;
    }
    else if (remove_point(node->SWB_,point))
    {
      return true;
    }
    else if (remove_point(node->SEF_,point))
    {
      return true;
    }
    else if (remove_point(node->SEB_,point))
    {
      return true;
    }
    else if (remove_point(node->NWF_,point))
    {
      return true;
    }
    else if (remove_point(node->NWB_,point))
    {
      return true;
    }
    else if (remove_point(node->NEF_,point))
    {
      return true;
    }
    else if (remove_point(node->NEB_,point))
    {
      return true;
    }
    return false;
  }


  void OcTree::subdivide(std::shared_ptr<OcTreeNode> node) 
  {
    node->divided_ = true;

    float new_width = node->width_ / 2;
    float new_height = node->height_ / 2;
    float new_depth = node->depth_ / 2;
    
    std::shared_ptr<Point3> SWF_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> SEF_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> NWF_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> NEF_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> SWB_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> SEB_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> NWB_center = std::make_shared<Point3>();
    std::shared_ptr<Point3> NEB_center = std::make_shared<Point3>();

    SWF_center->coord[0] = node->center_->coord[0] - node->width_ / 4;
    SWF_center->coord[1] = node->center_->coord[1] - node->height_ / 4;
    SWF_center->coord[2] = node->center_->coord[2] - node->depth_ / 4;
    node->SWF_.reset(
      new OcTreeNode(SWF_center,new_width,new_height,new_depth,node->capacity_)
      );

    SWB_center->coord[0] = node->center_->coord[0] - node->width_ / 4;
    SWB_center->coord[1] = node->center_->coord[1] - node->height_ / 4;
    SWB_center->coord[2] = node->center_->coord[2] + node->depth_ / 4;
    node->SWB_.reset(
      new OcTreeNode(SWB_center,new_width,new_height,new_depth,node->capacity_)
      );

    SEF_center->coord[0] = node->center_->coord[0] - node->width_ / 4;
    SEF_center->coord[1] = node->center_->coord[1] + node->height_ / 4;
    SEF_center->coord[2] = node->center_->coord[2] - node->depth_ / 4;
    node->SEF_.reset(
      new OcTreeNode(SEF_center,new_width,new_height,new_depth,node->capacity_)
      );

    SEB_center->coord[0] = node->center_->coord[0] - node->width_ / 4;
    SEB_center->coord[1] = node->center_->coord[1] + node->height_ / 4;
    SEB_center->coord[2] = node->center_->coord[2] + node->depth_ / 4;
    node->SEB_.reset(
      new OcTreeNode(SEB_center,new_width,new_height,new_depth,node->capacity_)
      );

    NWF_center->coord[0] = node->center_->coord[0] + node->width_ / 4;
    NWF_center->coord[1] = node->center_->coord[1] - node->height_ / 4;
    NWF_center->coord[2] = node->center_->coord[2] - node->depth_ / 4;
    node->NWF_.reset(
      new OcTreeNode(NWF_center,new_width,new_height,new_depth,node->capacity_)
      );

    NWB_center->coord[0] = node->center_->coord[0] + node->width_ / 4;
    NWB_center->coord[1] = node->center_->coord[1] - node->height_ / 4;
    NWB_center->coord[2] = node->center_->coord[2] + node->depth_ / 4;
    node->NWB_.reset(
      new OcTreeNode(NWB_center,new_width,new_height,new_depth,node->capacity_)
      );

    NEF_center->coord[0] = node->center_->coord[0] + node->width_ / 4;
    NEF_center->coord[1] = node->center_->coord[1] + node->height_ / 4;
    NEF_center->coord[2] = node->center_->coord[2] - node->depth_ / 4;
    node->NEF_.reset(
      new OcTreeNode(NEF_center,new_width,new_height,new_depth,node->capacity_)
      );

    NEB_center->coord[0] = node->center_->coord[0] + node->width_ / 4;
    NEB_center->coord[1] = node->center_->coord[1] + node->height_ / 4;
    NEB_center->coord[2] = node->center_->coord[2] + node->depth_ / 4;
    node->NEB_.reset(
      new OcTreeNode(NEB_center,new_width,new_height,new_depth,node->capacity_)
      );
  }

  bool OcTree::search(std::shared_ptr<OcTreeNode> node,
      Point3* point) const 
  {
    if (!node->inside_boundary(point)) 
    {
      return false;
    }
    // check all points at this node
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (fabsf(node->points_[i]->coord[0]-point->coord[0]) < FLT_NEAR_ZERO &&
          fabsf(node->points_[i]->coord[1]-point->coord[1]) < FLT_NEAR_ZERO &&
          fabsf(node->points_[i]->coord[2]-point->coord[2]) < FLT_NEAR_ZERO) 
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

    // Search everything, one of them will be righta
    if (search(node->SWF_,point))
    {
      return true;
    }
    else if (search(node->SWB_,point))
    {
      return true;
    }
    else if (search(node->SEF_,point))
    {
      return true;
    }
    else if (search(node->SEB_,point))
    {
      return true;
    }
    else if (search(node->NWF_,point))
    {
      return true;
    }
    else if (search(node->NWB_,point))
    {
      return true;
    }
    else if (search(node->NEF_,point))
    {
      return true;
    }
    else if (search(node->NEB_,point))
    {
      return true;
    }
    return false;
  }

  void OcTree::search_tree(
      std::vector<Point3>* points, std::shared_ptr<OcTreeNode> node, 
      Point3* center, float width, float height, float depth) const 
  {
    if (!node->intersect_boundary(center, width, height, depth)) 
    {
      return;
    }
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (node->points_[i]->coord[0] < center->coord[0] + width/2 &&
          node->points_[i]->coord[0] > center->coord[0] - width/2 &&
          node->points_[i]->coord[1] < center->coord[1] + height/2 &&
          node->points_[i]->coord[1] > center->coord[1] - height/2 && 
          node->points_[i]->coord[2] < center->coord[2] + depth/2 &&
          node->points_[i]->coord[2] > center->coord[2] - depth/2) 
      {
        Point3 temp;
        temp.coord[0] = node->points_[i]->coord[0];
        temp.coord[1] = node->points_[i]->coord[1];
        temp.coord[2] = node->points_[i]->coord[2];
        points->push_back(temp);        
      }
    }
    if (!node->divided_) 
    {
      return;
    }
    search_tree(points,node->SWF_,center,width,height,depth);
    search_tree(points,node->SEF_,center,width,height,depth);
    search_tree(points,node->NWF_,center,width,height,depth);
    search_tree(points,node->NEF_,center,width,height,depth);
    search_tree(points,node->SWB_,center,width,height,depth);
    search_tree(points,node->SEB_,center,width,height,depth);
    search_tree(points,node->NWB_,center,width,height,depth);
    search_tree(points,node->NEB_,center,width,height,depth);
  }
  
}
