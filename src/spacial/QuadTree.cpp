#include "glibby/spacial/QuadTree.h"
#include "glibby/primitives/point2D.h"
#include "glibby/math/general_math.h"

#include <memory>
#include <cmath>
#include <vector>


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
    this->capacity_ = cap;
    this->parent_ = NULL;
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

    this->ptr_ = other.ptr_;
    this->pos_ = other.pos_;

    return *this;
  }

  bool QuadTreeIterator::operator==(const QuadTreeIterator& other) const
  {
    return (this->ptr_ == other.ptr_ && this->pos_ == other.pos_);
  }

  bool QuadTreeIterator::operator!=(const QuadTreeIterator& other) const
  {
    if (this->ptr_ == NULL && other.ptr_ == NULL)
    {
      // these two are equal no matter the position
      return false;
    }
    if (this->ptr_ != other.ptr_)
    {
      return true;
    }
    return this->pos_ != other.pos_;
  }

  QuadTreeIterator& QuadTreeIterator::operator++()
  {
    if (ptr_ == NULL) 
    { // end of iteration, nothing left to look at
      return *this;
    }
    if (pos_ < ptr_->points_.size())
    { // still more to look at in this node, so don't leave yet
      pos_++;
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
      if (ptr_ == ptr_->parent_->SW_) 
      {
        ptr_ = ptr_->parent_->NW_;
        pos_ = 0;
        while (ptr_->SW_ != NULL)
        {
          ptr_ = ptr_->SW_;
        }
      }
      else if (ptr_ == ptr_->parent_->NW_) 
      {
        ptr_ = ptr_->parent_->NE_;
        pos_ = 0;
        while (ptr_->SW_ != NULL)
        {
          ptr_ = ptr_->SW_;
        }
      }
      if (ptr_ == ptr_->parent_->NE_) 
      {
        ptr_ = ptr_->parent_->SE_;
        pos_ = 0;
        while (ptr_->SW_ != NULL)
        {
          ptr_ = ptr_->SW_;
        }
      }
      if (ptr_ == ptr_->parent_->SE_) 
      {
        ptr_ = ptr_->parent_;
        pos_ = 0;
      }
    }

    // there is a chance that we just moved to a child that doesn't actually
    // contain any data. We need to fix that by iterating again
    if (pos_ == ptr_->points_.size()) 
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
    return this->ptr_->points_[this->pos_];
  }

  void find_deepest_node()
  {
    while (ptr_->SW_ != NULL)
    {
      // there are some children, just have to find which one has actual points
      if (ptr_->SW_->points_.size() != 0)
      {
        // there are points at this child
        ptr_ = ptr_->SW_;
      } 
      else if (ptr_->NW_->points_.size() != 0)
      {
        // there are points at this child
        ptr_ = ptr_->NW_;
      } 
      else if (ptr_->NE_->points_.size() != 0)
      {
        // there are points at this child
        ptr_ = ptr_->NE_;
      }
      else if (ptr_->SE_->points_.size() != 0)
      {
        ptr_ = ptr_->NE_;
      }
      else
      {
        // AAAH, we need to reformat the QuadTree because some point was deleted
        // and now part of the graph is "disconnected"
      }
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
    
    std::shared_ptr<Point2D> temp(new glibby::Point2D);
    temp->x = BotR->x + width/2;
    temp->y = BotR->y + height/2;

    this->root_ = std::shared_ptr<QuadTreeNode>(
        new QuadTreeNode(temp, width, height, capacity));
  }
   
  QuadTree::QuadTree(std::shared_ptr<Point2D> p, float width, float height, 
      int capacity) 
  {
    std::shared_ptr<Point2D> temp(new Point2D);
    temp->x = p->x;
    temp->y = p->y;
    this->root_ = std::shared_ptr<QuadTreeNode>(
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

  QuadTree::iterator QuadTree::begin() const
  {
    QuadTreeNode* p = &(*root_);
    while (p->SW_ != NULL)
    {
      p = &(*(p->SW_));
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
    return remove_point(this->root_, point);
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
    temp.reset( new QuadTreeNode(
        this->root_->center_,
        this->root_->width_,
        this->root_->height_,
        this->root_->capacity_));
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
    // if we have space at this node, add it here
    if (node->points_.size() < node->capacity_) 
    {
      node->points_.push_back(std::shared_ptr<Point2D>(new Point2D));
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
    node->SW_->parent_ = node;

    SE_center->x = node->center_->x + node->width_ / 4;
    SE_center->y = node->center_->y - node->height_ / 4;
    node->SE_.reset(
        new QuadTreeNode(SE_center,new_width,new_height,node->capacity_)
        );
    node->SE_->parent_ = node;

    NW_center->x = node->center_->x - node->width_ / 4;
    NW_center->y = node->center_->y + node->height_ / 4;
    node->NW_.reset(
        new QuadTreeNode(NW_center,new_width,new_height,node->capacity_)
        );
    node->NW_->parent_ = node;

    NE_center->x = node->center_->x + node->width_ / 4;
    NE_center->y = node->center_->y + node->height_ / 4;
    node->NE_.reset(
        new QuadTreeNode(NE_center,new_width,new_height,node->capacity_)
        );
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
  
}
