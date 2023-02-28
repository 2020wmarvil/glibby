#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "glibby/primitives/point2D.h"

#include <memory>
#include <vector>

namespace glibby 
{
  class QuadTreeNode 
  {
    public:
      QuadTreeNode(std::shared_ptr<Point2D> cen, float width, float height, 
          int cap);

      bool inside_boundary(Point2D* ptr) const;
      bool intersect_boundary(Point2D* center, float width, float height) const;

      std::shared_ptr<Point2D> center_;
      long unsigned capacity_;
      float width_;
      float height_;
      bool divided_;
      std::shared_ptr<QuadTreeNode> NW_;
      std::shared_ptr<QuadTreeNode> NE_;
      std::shared_ptr<QuadTreeNode> SW_;
      std::shared_ptr<QuadTreeNode> SE_;
      std::shared_ptr<QuadTreeNode> parent_; 
      std::vector<std::shared_ptr<Point2D>> points_;
  };

  class QuadTree 
  {
    public:
      QuadTree(std::shared_ptr<Point2D> TopL, std::shared_ptr<Point2D> TopR, 
          std::shared_ptr<Point2D> BotL, std::shared_ptr<Point2D> BotR, 
          int capacity = 1);
      QuadTree(std::shared_ptr<Point2D> p, float width, float height, 
          int capacity = 1);

      bool insert(Point2D* point);
      bool contains(Point2D* point) const;
      std::vector<Point2D> query(Point2D* point, float width, 
          float height) const;

      int size() const {return size_;};

    private:
      bool add_point(std::shared_ptr<QuadTreeNode> node, Point2D* point);
      void subdivide(std::shared_ptr<QuadTreeNode> node);
      bool search(std::shared_ptr<QuadTreeNode> node, Point2D* point) const;
      void search_tree(
          std::vector<Point2D>* points, std::shared_ptr<QuadTreeNode> node, 
          Point2D* center, float width, float height) const;
      
      std::shared_ptr<QuadTreeNode> node_;
      int capacity_;
      int size_;
  };
}

#endif
