#include "glibby/spacial/OcTree.h"
#include "glibby/primitives/point2D.h"
#include "glibby/math/general_math.h"

#include <memory>
#include <cmath>
#include <vector>


/*
 * NAVIGATING THE CUBE
 *
 * since Point3 is used, each point has 3 values: points[0], points[1],points[2]
 *
 * points[0] controls North vs South (North is positive or up, South is negative
 * or down)
 *
 * points[1] controls East vs West (East is positive or right, West is negative
 * or left)
 *
 * points[2] controls Back vs Front (Back is positive, Front is negative)
 *
 * it is important to keep this consistent throughout
 */


namespace glibby 
{
  /*
   *
   * OC TREE NODE CLASS
   *
   * */
  OcTreeNode::OcTreeNode(std::shared_ptr<Point3> cen, float width, 
      float height, float depth, int cap) 
  {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->depth_ = depth;
    this->divided_ = false;
    this->capacity_ = cap;
    //this->parent_ = NULL;
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

    if (thisLeft > otherRight || otherLeft > thisRight ||
        thisBottom > otherTop || otherBottom > thisTop ||
        thisBack > otherFront || otherBack > thisFront)
    {
      return false;
    }
    return true;
  }

  /*
   *
   * OC TREE ITERATOR CLASS
   *
   * */
  OcTreeIterator& OcTreeIterator::operator=(const OcTreeIterator& other)
  {
    // no self assignment
    if (this == &other)
    {
      return *this;
    }
    
    auto other_ptr = other.ptr_.lock();
    this->ptr_ = other_ptr;
    this->pos_ = other.pos_;

    return *this;
  }

  bool OcTreeIterator::operator==(const OcTreeIterator& other) const
  {
    auto this_ptr = this->ptr_.lock();
    auto other_ptr = other.ptr_.lock();
    if (this_ptr == NULL && other_ptr == NULL)
    { // these two will always be equal no matter what
      return true;
    }
    return (this_ptr == other_ptr && this->pos_ == other.pos_);
  }

  bool OcTreeIterator::operator!=(const OcTreeIterator& other) const
  {
    return !(*this == other);
  }

  OcTreeIterator& OcTreeIterator::operator++()
  {
    /*
     * Traversing tree through post-order
     * visit all subtree nodes, then visit root
     * */
    auto ptr = ptr_.lock();
    auto parent = ptr->parent_.lock();
    if (ptr == NULL) 
    { // end of iteration, nothing left to look at
      return *this;
    }
    if (pos_ < ptr->points_.size()-1 && ptr->points_.size() != 0)
    { // still more to look at in this node, so don't leave yet
      pos_++;
      return *this;
    }
    if (parent == NULL)
    { // we have reached the root, nothing more to look at
      ptr_ = std::weak_ptr<OcTreeNode>();
      pos_ = 0;
      return *this;
    }
    else 
    { // nothing more at this node, move to next
      /*
       * if we are in the parent's SWB node, -> SWB-most node of parent's NWB
       * if we are in the parent's NWB node, -> SWB-most node of parent's NEB
       * if we are in the parent's NEB node, -> SWB-most node of parent's SEB
       * if we are in the parent's SEB node, -> SWB-most node of parent's SWF
       * if we are in the parent's SWF node, -> SWB-most node of parent's NWF
       * if we are in the parent's NWF node, -> SWB-most node of parent's NEF
       * if we are in the parent's NEF node, -> SWB-most node of parent's SEF
       * if we are in the parent's SEF node, -> parent
       *
       * we will never have to check if the parent does not have a SW/NW/NE/SE
       * because when we subdivide a node, all of the children are created at
       * once
       * */
      if (ptr == parent->SWB_) 
      {
        ptr_ = parent->NWB_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->NWB_) 
      {
        ptr_ = parent->NEB_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->NEB_) 
      {
        ptr_ = parent->SEB_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->SEB_) 
      {
        ptr_ = parent->SWF_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->SWF_) 
      {
        ptr_ = parent->NWF_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->NWF_) 
      {
        ptr_ = parent->NEF_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->NEF_) 
      {
        ptr_ = parent->SEF_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->SEF_) 
      {
        ptr_ = parent;
        pos_ = 0;
      }
    }

    // there is a chance that we just moved to a child that doesn't actually
    // contain any data. We need to fix that by iterating again
    ptr = ptr_.lock();
    if (ptr->points_.size() == 0) 
    {
      ++(*this);
    }

    return *this;
  }

  OcTreeIterator OcTreeIterator::operator++(int)
  {
    OcTreeIterator temp;
    temp = *this;
    ++(*this);
    return temp;
  }

  const std::shared_ptr<const Point3> OcTreeIterator::operator*() const
  {
    auto ptr = ptr_.lock();
    return ptr->points_[this->pos_];
  }

  void OcTreeIterator::find_deepest_child()
  {
    auto ptr = ptr_.lock();
    while (ptr->SWB_ != NULL)
    { // since we subdivide everything at once, if one child exists, all do
      if (ptr->SWB_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->SWB_;
      }
      else if (ptr->NWB_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->NWB_;
      }
      else if (ptr->NEB_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->NEB_;
      }
      else if (ptr->SEB_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->SEB_;
      }
      else if (ptr->SWF_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->SWF_;
      }
      else if (ptr->NWF_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->NWF_;
      }
      else if (ptr->NEF_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->NEF_;
      }
      else if (ptr->SEF_->points_.size() != 0)
      {
        // there are points here, this child has highest priority so far, go
        // here
        ptr_ = ptr->SEF_;
      }
      else
      {
        // should not be possible, means we subdivided with no actual reason to
        // do so
      }
      ptr = ptr_.lock();
    }
  }


  /*
   *
   * OC TREE CLASS
   *
   * */
   OcTree::OcTree(std::shared_ptr<Point3> p, float width, float height, 
      float depth, int capacity) 
  {
    std::shared_ptr<Point3> temp = std::make_shared<Point3>();
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
    this->root_ = std::make_shared<OcTreeNode>(
        temp,width,height,depth,this->capacity_);
    this->size_ = 0;
  }

  OcTree::iterator OcTree::begin() const
  {
    std::shared_ptr<OcTreeNode> p = root_;
    while (p->SWB_ != NULL)
    {
      p = p->SWB_;
    }
    OcTree::iterator temp(p,0);
    // just in case the node we just moved to does not contain any data
    if (p->points_.size() == 0)
    {
      temp++;
    }
    return temp;
  }

  OcTree::iterator OcTree::end() const
  {
    return OcTree::iterator(NULL,0);
  }

 std::pair<bool,OcTree::iterator> OcTree::insert(Point3* point) 
  {
    // out of bounds
    if (!this->root_->inside_boundary(point)) 
    {
      return std::pair<bool,OcTree::iterator>(false,OcTree::iterator());
    }
    return add_point(this->root_, point);
  }

  bool OcTree::remove(Point3* point) 
  {
    // out of bounds
    if (!this->root_->inside_boundary(point)) 
    {
      return false;
    }
    bool temp = remove_point(this->root_, point);

    std::vector<Point3> points = query(&(*(this->root_->center_)),
        (*this).root_->width_+1,(*this).root_->height_+1,
        (*this).root_->depth_+1);
    reformat_tree(points);

    return temp;
  }

  std::pair<bool,OcTree::iterator> OcTree::contains(Point3* point) const 
  {
    if (!this->root_->inside_boundary(point)) 
    {
      return std::pair<bool,OcTree::iterator>(false,OcTree::iterator());
    }
    return search(this->root_,point);
  }

  std::vector<Point3> OcTree::query (Point3* point, 
      float width, float height, float depth) const 
  {
    std::vector<Point3> points;
    search_tree(&points, this->root_, point, width, height, depth);
    return points;
  }

  void OcTree::clear() {
    this->size_ = 0;
    std::shared_ptr<OcTreeNode> temp;
    temp = std::make_shared<OcTreeNode>(
        this->root_->center_,
        this->root_->width_,
        this->root_->height_,
        this->root_->depth_,
        this->root_->capacity_);
    this->root_ = temp;
  }



  std::pair<bool,OcTree::iterator> OcTree::add_point(
      std::shared_ptr<OcTreeNode> node, Point3* point) 
  {
    // is it inside the boundary of the node?
    if (!node->inside_boundary(point)) 
    {
      return std::pair<bool,OcTree::iterator>(false,OcTree::iterator());
    }
    // if we have space at this node, add it here
    if (node->points_.size() < node->capacity_) 
    {
      node->points_.push_back(std::make_shared<Point3>());
      node->points_[node->points_.size()-1]->points[0] = point->points[0];
      node->points_[node->points_.size()-1]->points[1] = point->points[1];
      node->points_[node->points_.size()-1]->points[2] = point->points[2];
      this->size_++;
      OcTree::iterator temp(node,node->points_.size()-1);
      return std::pair<bool,OcTree::iterator>(true,temp);
    }
    
    // if we need to subdivide this node, do so
    if (!node->divided_) 
    {
      subdivide(node);
    }

    // Add to everything, one of them will be right
    std::pair<bool,OcTree::iterator> temp;
    temp = add_point(node->SWF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->SWB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->SEF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->SEB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->NWF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->NWB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->NEF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = add_point(node->NEB_,point);
    if (temp.first)
    {
      return temp;
    }
    return std::pair<bool,OcTree::iterator>(false,OcTree::iterator());
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
      if (node->points_[i]->points[0] == point->points[0] && 
          node->points_[i]->points[1] == point->points[1] &&
          node->points_[i]->points[2] == point->points[2]) 
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

    SWF_center->points[0] = node->center_->points[0] - node->width_ / 4;
    SWF_center->points[1] = node->center_->points[1] - node->height_ / 4;
    SWF_center->points[2] = node->center_->points[2] - node->depth_ / 4;
    node->SWF_ = std::make_shared<OcTreeNode>(
      SWF_center,new_width,new_height,new_depth,node->capacity_);
    node->SWF_->parent_ = node;

    SWB_center->points[0] = node->center_->points[0] - node->width_ / 4;
    SWB_center->points[1] = node->center_->points[1] - node->height_ / 4;
    SWB_center->points[2] = node->center_->points[2] + node->depth_ / 4;
    node->SWB_ = std::make_shared<OcTreeNode>(
      SWB_center,new_width,new_height,new_depth,node->capacity_);
    node->SWB_->parent_ = node;

    SEF_center->points[0] = node->center_->points[0] - node->width_ / 4;
    SEF_center->points[1] = node->center_->points[1] + node->height_ / 4;
    SEF_center->points[2] = node->center_->points[2] - node->depth_ / 4;
    node->SEF_ = std::make_shared<OcTreeNode>(
      SEF_center,new_width,new_height,new_depth,node->capacity_);
    node->SEF_->parent_ = node;

    SEB_center->points[0] = node->center_->points[0] - node->width_ / 4;
    SEB_center->points[1] = node->center_->points[1] + node->height_ / 4;
    SEB_center->points[2] = node->center_->points[2] + node->depth_ / 4;
    node->SEB_ = std::make_shared<OcTreeNode>(
      SEB_center,new_width,new_height,new_depth,node->capacity_);
    node->SEB_->parent_ = node;

    NWF_center->points[0] = node->center_->points[0] + node->width_ / 4;
    NWF_center->points[1] = node->center_->points[1] - node->height_ / 4;
    NWF_center->points[2] = node->center_->points[2] - node->depth_ / 4;
    node->NWF_ = std::make_shared<OcTreeNode>(
      NWF_center,new_width,new_height,new_depth,node->capacity_);
    node->NWF_->parent_ = node;

    NWB_center->points[0] = node->center_->points[0] + node->width_ / 4;
    NWB_center->points[1] = node->center_->points[1] - node->height_ / 4;
    NWB_center->points[2] = node->center_->points[2] + node->depth_ / 4;
    node->NWB_ = std::make_shared<OcTreeNode>(
      NWB_center,new_width,new_height,new_depth,node->capacity_);
    node->NWB_->parent_ = node;

    NEF_center->points[0] = node->center_->points[0] + node->width_ / 4;
    NEF_center->points[1] = node->center_->points[1] + node->height_ / 4;
    NEF_center->points[2] = node->center_->points[2] - node->depth_ / 4;
    node->NEF_ = std::make_shared<OcTreeNode>(
      NEF_center,new_width,new_height,new_depth,node->capacity_);
    node->NEF_->parent_ = node;

    NEB_center->points[0] = node->center_->points[0] + node->width_ / 4;
    NEB_center->points[1] = node->center_->points[1] + node->height_ / 4;
    NEB_center->points[2] = node->center_->points[2] + node->depth_ / 4;
    node->NEB_ = std::make_shared<OcTreeNode>(
      NEB_center,new_width,new_height,new_depth,node->capacity_);
    node->NEB_->parent_ = node;
  }

  std::pair<bool,OcTree::iterator> OcTree::search(std::shared_ptr<OcTreeNode> node,
      Point3* point) const 
  {
    if (!node->inside_boundary(point)) 
    {
      OcTree::iterator temp = this->end();
      return std::pair<bool,OcTree::iterator>(false,temp);
    }
    // check all points at this node
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (fabsf(node->points_[i]->points[0]-point->points[0]) < FLT_NEAR_ZERO &&
          fabsf(node->points_[i]->points[1]-point->points[1]) < FLT_NEAR_ZERO &&
          fabsf(node->points_[i]->points[2]-point->points[2]) < FLT_NEAR_ZERO) 
      {
        OcTree::iterator temp(node,i);
        return std::pair<bool,OcTree::iterator>(true,temp);
      } 
    }
    // not at this node, so check sub-node it might be in, but first
    // check if we actually have any sub-nodes
    if (!node->divided_) 
    {
      OcTree::iterator temp = this->end();
      return std::pair<bool,OcTree::iterator>(false,temp);
    }

    // Search everything, one of them will be right
    std::pair<bool,OcTree::iterator> temp = search(node->SWF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->SWB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->SEF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->SEB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->NWF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->NWB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->NEF_,point);
    if (temp.first)
    {
      return temp;
    }
    temp = search(node->NEB_,point);
    if (temp.first)
    {
      return temp;
    }
    temp.second = this->end();
    return std::pair<bool,OcTree::iterator>(false,temp.second);
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
      if (node->points_[i]->points[0] < center->points[0] + width/2 &&
          node->points_[i]->points[0] > center->points[0] - width/2 &&
          node->points_[i]->points[1] < center->points[1] + height/2 &&
          node->points_[i]->points[1] > center->points[1] - height/2 && 
          node->points_[i]->points[2] < center->points[2] + depth/2 &&
          node->points_[i]->points[2] > center->points[2] - depth/2) 
      {
        Point3 temp;
        temp.points[0] = node->points_[i]->points[0];
        temp.points[1] = node->points_[i]->points[1];
        temp.points[2] = node->points_[i]->points[2];
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

  void OcTree::reformat_tree(std::vector<Point3> points) 
  {
    std::shared_ptr<OcTreeNode> temp = std::make_shared<OcTreeNode>(
          (*this).root_->center_, (*this).root_->width_, (*this).root_->height_,
          (*this).root_->depth_, (*this).root_->capacity_);

    this->root_ = temp;

    for (unsigned int i=0; i < points.size(); i++)
    {
      this->insert(&points[i]);
    }
  }
  
}
