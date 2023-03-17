#ifndef OC_TREE_H
#define OC_TREE_H

#include "glibby/primitives/point.h"

#include <memory>
#include <vector>

namespace glibby 
{
  class OcTreeNode 
  {
    public:
      OcTreeNode(std::shared_ptr<Point3> cen, float width, float height, 
          float depth, int cap);

      bool inside_boundary(Point3* ptr) const;
      bool intersect_boundary(Point3* center, float width, float height, 
          float depth) const;

      std::shared_ptr<Point3> center_;
      long unsigned capacity_;
      float width_;
      float height_;
      float depth_;
      bool divided_;
      std::shared_ptr<OcTreeNode> NWF_;
      std::shared_ptr<OcTreeNode> NEF_;
      std::shared_ptr<OcTreeNode> SWF_;
      std::shared_ptr<OcTreeNode> SEF_;
      std::shared_ptr<OcTreeNode> NWB_;
      std::shared_ptr<OcTreeNode> NEB_;
      std::shared_ptr<OcTreeNode> SWB_;
      std::shared_ptr<OcTreeNode> SEB_;
      std::vector<std::shared_ptr<Point3>> points_;
  };

  class OcTree 
  {
    public:
      OcTree(std::shared_ptr<Point3> p, float width, float height, float depth,
          int capacity = 1);

      bool insert(Point3* point);
      bool remove(Point3* point);
      bool contains(Point3* point) const;
      std::vector<Point3> query(Point3* point, float width, float height, 
          float depth) const;

      int size() const {return size_;};
      void clear();

    private:
      bool add_point(std::shared_ptr<OcTreeNode> node, Point3* point);
      bool remove_point(std::shared_ptr<OcTreeNode> node, Point3* point);
      void subdivide(std::shared_ptr<OcTreeNode> node);
      bool search(std::shared_ptr<OcTreeNode> node, Point3* point) const;
      void search_tree(
          std::vector<Point3>* points, std::shared_ptr<OcTreeNode> node, 
          Point3* center, float width, float height, float depth) const;
      
      std::shared_ptr<OcTreeNode> node_;
      int capacity_;
      int size_;
  };
}

#endif
