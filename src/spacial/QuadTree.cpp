#include "glibby/spacial/QuadTree.h"
#include "glibby/primitives/point2D.h"
#include "glibby/math/general_math.h"

#include <memory>
#include <cmath>
#include <vector>

// TODO: enforce all points are leaves

namespace glibby 
{
  /*
   *
   * QUAD TREE NODE CLASS
   *
   * */
  QuadTreeNode::QuadTreeNode(std::shared_ptr<Point2D> cen, float width, 
      float height, int cap) 
  {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->divided_ = false;
    this->leaf_ = true;
    this->capacity_ = cap;
    //this->parent_ = NULL;
  }

  bool QuadTreeNode::inside_boundary(Point2D* ptr) const 
  {
    if (ptr->x <= this->center_->x + this->width_ / 2 &&
        ptr->x >= this->center_->x - this->width_ / 2 &&
        ptr->y <= this->center_->y + this->height_ / 2 &&
        ptr->y >= this->center_->y - this->height_ / 2) 
    {
      return true;
    }
    return false;
  }

  bool QuadTreeNode::intersect_boundary(Point2D* center,
      float width, float height) const 
  {
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

    if (otherL > thisR || otherR < thisL || otherT < thisB || otherB > thisT) 
    {
      return false;
    }
    return true;
  }

  /*
   *
   * QUAD TREE ITERATOR CLASS
   *
   * */
  QuadTreeIterator& QuadTreeIterator::operator=(const QuadTreeIterator& other)
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

  bool QuadTreeIterator::operator==(const QuadTreeIterator& other) const
  { 
    auto this_ptr = this->ptr_.lock();
    auto other_ptr = other.ptr_.lock();
    if (this_ptr == NULL && other_ptr == NULL) 
    { // these two will always be equal no matter what
      return true;
    }
    return (this_ptr == other_ptr && this->pos_ == other.pos_);
  }

  bool QuadTreeIterator::operator!=(const QuadTreeIterator& other) const
  {
    return !(*this == other);
  }

  QuadTreeIterator& QuadTreeIterator::operator++()
  {
    /*
     * Traversing tree through post-order
     * visit all subtree node, then visit root
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
      ptr_ = std::weak_ptr<QuadTreeNode>();
      pos_ = 0;
      return *this;
    }
    else 
    { // nothing more at this node, move to next
      /*
       * if we are in the parent's SW node, we go to SW-most node of parent's NW
       * if we are in the parent's NW node, we go to SW-most node of parent's NE
       * if we are in the parent's NE node, we go to SW-most node of parent's SE
       * if we are in the parent's SE node, we go to parent
       *
       * we will never have to check if the parent does not have a SW/NW/NE/SE
       * because when we subdivide a node, all of the children are created at
       * once
       * */
      if (ptr == parent->SW_) 
      {
        ptr_ = parent->NW_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->NW_) 
      {
        ptr_ = parent->NE_;
        pos_ = 0;
        find_deepest_child();
      }
      else if (ptr == parent->NE_) 
      {
        ptr_ = parent->SE_;
        pos_ = 0;
        find_deepest_child();
      }
       else 
      {
        ptr_ = ptr->parent_;
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

  QuadTreeIterator QuadTreeIterator::operator++(int)
  {
    QuadTreeIterator temp;
    temp = *this;
    ++(*this);
    return temp;
  }

  const std::shared_ptr<const Point2D> QuadTreeIterator::operator*() const
  {
    auto ptr = this->ptr_.lock();
    return ptr->points_[this->pos_];
  }

  void QuadTreeIterator::find_deepest_child()
  {
    auto ptr = ptr_.lock();
    while (ptr->divided_)
    {
      // there are some children, just have to find which one has actual points
      // when we iterate, we check to make sure the node we moved to has points,
      // so if we move to an empty node here, it will be taken care of by that
      // process
      ptr_ = ptr->SW_;
      ptr = ptr_.lock();
    }
  }


  /*
   *
   * QUAD TREE CLASS
   *
   * */
  QuadTree::QuadTree(std::shared_ptr<Point2D> TopL, 
      std::shared_ptr<Point2D> TopR, std::shared_ptr<Point2D> BotL, 
      std::shared_ptr<Point2D> BotR, int capacity) 
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
    float width = TopR->x - TopL->x;
    float height = TopR->y - BotR->y;
    
    std::shared_ptr<Point2D> temp = std::make_shared<Point2D>();
    temp->x = BotR->x + width/2;
    temp->y = BotR->y + height/2;

    this->root_ = std::make_shared<QuadTreeNode>(
        temp, width, height, capacity);
  }
   
  QuadTree::QuadTree(std::shared_ptr<Point2D> p, float width, float height, 
      int capacity) 
  {
    std::shared_ptr<Point2D> temp = std::make_shared<Point2D>();
    temp->x = p->x;
    temp->y = p->y;
    this->root_ = std::make_shared<QuadTreeNode>(
        temp,width,height,capacity);
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

  QuadTree::iterator QuadTree::begin() const
  {
    std::shared_ptr<QuadTreeNode> p = root_;
    while (p->divided_)
    {
      p = p->SW_;
    }
    QuadTree::iterator temp(p,0);
    // just in case the node we just moved to does not contain any data
    if (p->points_.size() == 0)
    {
      temp++;
    }
    return temp;
  }

  QuadTree::iterator QuadTree::end() const
  {
    return QuadTree::iterator();
  }

  std::pair<bool,QuadTree::iterator> QuadTree::insert(Point2D* point) 
  {
    // out of bounds
    if (!this->root_->inside_boundary(point)) 
    {
      QuadTree::iterator temp = this->end();
      return std::pair<bool,iterator>(false,temp);
    }
    return add_point(this->root_, point);
  }

  bool QuadTree::remove(Point2D* point) 
  {
    // out of bounds
    if (!this->root_->inside_boundary(point)) 
    {
      return false;
    }
    bool temp = remove_point(this->root_, point);

    /*std::vector<Point2D> points = query(&(*(this->root_->center_)),
        (*this).root_->width_+1,(*this).root_->height_+1);
    reformat_tree(points);*/

    return temp;
  }

  std::pair<bool,QuadTree::iterator> QuadTree::contains(Point2D* point) const 
  {
    if (!this->root_->inside_boundary(point)) 
    {
      QuadTree::iterator temp = this->end();
      return std::pair<bool,iterator>(false,temp);
    }
    return search(this->root_,point);
  }

  std::vector<Point2D> QuadTree::query (Point2D* point, 
      float width, float height) const 
  {
    std::vector<Point2D> points;
    search_tree(&points, this->root_, point, width, height);
    return points;
  }

  void QuadTree::clear() {
    this->size_ = 0;
    std::shared_ptr<QuadTreeNode> temp;
    temp = std::make_shared<QuadTreeNode>(
        this->root_->center_,
        this->root_->width_,
        this->root_->height_,
        this->root_->capacity_);
    this->root_ = temp;
  }



  std::pair<bool,QuadTree::iterator> QuadTree::add_point(
      std::shared_ptr<QuadTreeNode> node, Point2D* point) 
  {
    // is it inside the boundary of the node?
    if (!node->inside_boundary(point)) 
    {
      QuadTree::iterator temp = this->end();
      return std::pair<bool,iterator>(false,temp);
    }
    // if node is leaf and we have space at this node, add it here
    if (node->leaf_ && node->points_.size() < node->capacity_) 
    {
      node->points_.push_back(std::make_shared<Point2D>());
      node->points_[node->points_.size()-1]->x = point->x;
      node->points_[node->points_.size()-1]->y = point->y;
      this->size_++;
      QuadTree::iterator temp(node,node->points_.size()-1);
      return std::pair<bool,iterator>(true,temp);
    }
    
    // if we need to subdivide this node, do so
    if (!node->divided_) 
    {
      subdivide(node);
      // we subdivided this node, meaning it is no longer a leaf, we need to fix
      // that by pushing whatever point(s) further down the tree to a leaf
      node->leaf_ = false;
      // push everything down by just adding the point(s) starting at current 
      // node will not be re-added to this node because now set as not leaf
      for (unsigned int i=0; i < node->points_.size(); i++)
      {
        add_point(node,&(*node->points_[i]));
      }
      // remove all the points from here
      node->points_.clear();
    }
    // are we to the left (W) of center
    if (point->x < node->center_->x) 
    {
      // above/below (N/S) of center
      if (point->y < node->center_->y) 
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
      if (point->y < node->center_->y) 
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
      Point2D* point) 
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


  void QuadTree::subdivide(std::shared_ptr<QuadTreeNode> node) 
  {
    node->divided_ = true;
    node->leaf_ = false;

    float new_width = node->width_ / 2;
    float new_height = node->height_ / 2;
    
    std::shared_ptr<Point2D> SW_center = std::make_shared<Point2D>();
    std::shared_ptr<Point2D> SE_center = std::make_shared<Point2D>();
    std::shared_ptr<Point2D> NW_center = std::make_shared<Point2D>();
    std::shared_ptr<Point2D> NE_center = std::make_shared<Point2D>();

    SW_center->x = node->center_->x - node->width_ / 4;
    SW_center->y = node->center_->y - node->height_ / 4;
    node->SW_ = std::make_shared<QuadTreeNode>(
        SW_center,new_width,new_height,node->capacity_);
    node->SW_->parent_ = node;

    SE_center->x = node->center_->x + node->width_ / 4;
    SE_center->y = node->center_->y - node->height_ / 4;
    node->SE_ = std::make_shared<QuadTreeNode>(
        SE_center,new_width,new_height,node->capacity_);
    node->SE_->parent_ = node;

    NW_center->x = node->center_->x - node->width_ / 4;
    NW_center->y = node->center_->y + node->height_ / 4;
    node->NW_ = std::make_shared<QuadTreeNode>(
        NW_center,new_width,new_height,node->capacity_);
    node->NW_->parent_ = node;

    NE_center->x = node->center_->x + node->width_ / 4;
    NE_center->y = node->center_->y + node->height_ / 4;
    node->NE_ = std::make_shared<QuadTreeNode>(
        NE_center,new_width,new_height,node->capacity_);
    node->NE_->parent_ = node;
  }

  std::pair<bool,QuadTree::iterator> QuadTree::search(
      std::shared_ptr<QuadTreeNode> node, Point2D* point) const 
  {
    if (!node->inside_boundary(point)) 
    {
      QuadTree::iterator temp = this->end();
      return std::pair<bool,iterator>(false,temp);
    }
    // check all points at this node
    for (long unsigned i=0; i < node->points_.size(); i++) 
    {
      if (fabsf(node->points_[i]->x - point->x) < FLT_NEAR_ZERO  &&
          fabsf(node->points_[i]->y - point->y) < FLT_NEAR_ZERO) 
      {
        QuadTree::iterator temp(node,i);
        return std::pair<bool,iterator>(true,temp);
      } 
    }
    // not at this node, so check sub-node it might be in, but first
    // check if we actually have any sub-nodes
    if (!node->divided_) 
    {
      QuadTree::iterator temp = this->end();
      return std::pair<bool,iterator>(false,temp);
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

  void QuadTree::search_tree(
      std::vector<Point2D>* points, std::shared_ptr<QuadTreeNode> node, 
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

  void QuadTree::reformat_tree(std::vector<Point2D> points)
  {
    std::shared_ptr<QuadTreeNode> temp = std::make_shared<QuadTreeNode>(
        (*this).root_->center_, (*this).root_->width_, (*this).root_->height_,
        (*this).root_->capacity_);

    this->root_ = temp;

    for (unsigned int i=0; i < points.size(); i++) 
    {
      this->insert(&points[i]);
    }
  }
  // TODO: add function to trim tree (if node has leaf children, but none of 
  // them have points, remove the children and make the node a leaf) 
}
