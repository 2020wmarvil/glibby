#include "glibby/spacial-algorithms/QuadTree.h"
#include "glibby/primitives/point2D.h"


namespace glibby {
  QuadTreeNode::QuadTreeNode(Point2D* cen, float width, float height, int cap) {
    this->center_ = cen;
    this->width_ = width;
    this->height_ = height;
    this->capacity_ = cap;
  }

  QuadTree::QuadTree(Point2D* TopL, Point2D* TopR, Point2D* BotL, Point2D* BotR, int capacity) {
    this->capacity_ = capacity;

    // using points to create rectangle that will be the bounding box
    float width = TopR->x - TopL->x;
    float height = TopR->y - BotR->y;
    
    Point2D* temp = new Point2D;
    temp->x = BotR->x + width/2;
    temp->y = BotR->y + height/2;

    this->ptr_ = new QuadTreeNode(temp, width, height, capacity);
  }
   
  QuadTree::QuadTree(Point2D* p, float width, float height, int capacity) {
    this->ptr_ = new QuadTreeNode(p,width,height,capacity);
    this->capacity_ = capacity;
  }

  QuadTree::~QuadTree(){
    destroy_sub_tree(this->ptr_);
  }

  

  void QuadTree::destroy_sub_tree(QuadTreeNode* ptr) {
    // if ptr is NULL we stop recursing
    if (ptr == 0) {
      return;
    }
    delete ptr->center_;
    delete [] ptr->points_;
    // recurse to all subtrees
    destroy_sub_tree(ptr->NW_);
    destroy_sub_tree(ptr->NE_);
    destroy_sub_tree(ptr->SW_);
    destroy_sub_tree(ptr->SE_);
  }
}
