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
      /**
       * @brief Constructor for OcTreeNode.
       *
       * Each OcTreeNode has a boundary defining an area from which points
       * can be inserted. Points lying outside that area are not valid, and
       * should not be added to the node.
       *
       * Additionally, each node has a capacity for number of nodes which 
       * should not be passed in order to retain an even distribution of points
       * throughout the QuadTree.
       *
       * Each node has eight sub-nodes (NorthEastFront, NorthWestFront, 
       * SouthEastFront, SouthWestFront, NorthEastBack, NorthWestBack, 
       * SouthEastBack, SouthWestBack). Each sub-node's area is contained 
       * within the parent's area.
       *
       * @param cen - shared pointer to Point3, center of the area
       * @param width - overall width of area
       * @param height - overall height of area
       * @param depth - overall depth of area
       * @param cap - capacity of the OcTreeNode
       * */
      OcTreeNode(std::shared_ptr<Point3> cen, float width, float height, 
          float depth, int cap);

      /**
       * @brief Check if Point3 is inside the boundary defined by the OcTreeNode
       *
       * @param ptr - pointer to Point3
       *
       * @return true if inside boundary, false otherwise
       * */
      bool inside_boundary(Point3* ptr) const;
      /**
       * @brief Check if boundary defined by the parameters will intersect with
       * the boundary defined by the OcTreeNode
       *
       * @param center - pointer to Poin3, center of volume contained by
       * boundary
       * @param width - overall width of the boundary, the left edge is center -
       * width / 2, the right edge is center + width / 2
       * @param height - overall height of the boundary, the top edge is 
       * center + height / 2, the bottom edge is center - height / 2
       * @param depth - overall depth of the boundary, the back edge is center -
       * depth / 2, the front edge is center + depth / 2
       * 
       * @return true if boundaries intersect, false otherwise
       * */
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
      std::shared_ptr<OcTreeNode> parent_;
      std::vector<std::shared_ptr<Point3>> points_;
  };

  class OcTreeIterator
  {
    public:
      OcTreeIterator() : ptr_(NULL), pos_(0) {};
      OcTreeIterator(OcTreeNode* ptr, unsigned int pos) : ptr_(ptr), pos_(pos) {};
      OcTreeIterator(std::shared_ptr<OcTreeNode> ptr, unsigned int pos) : ptr_(ptr), pos_(pos) {};
      ~OcTreeIterator() {};
      OcTreeIterator& operator=(const OcTreeIterator& other);

      bool operator==(const OcTreeIterator& other) const;
      bool operator!=(const OcTreeIterator& other) const;

      OcTreeIterator& operator++();
      OcTreeIterator operator++(int);

      const std::shared_ptr<const Point3> operator*() const;

    private:
      std::shared_ptr<OcTreeNode> ptr_;
      unsigned int pos_;
  };

  class OcTree 
  {
    public:
      /**
       * @brief Creates OcTree with boundary defined by the parameters, the
       * boundar is cubic
       *
       * @param p - pointer to the Point3 which is center of boundary
       * @param width - overall width of the boundary
       * @param height - overall height of the boundary
       * @param depth - overall depth of the boundary
       * @param capacity - capacity of each node in tree, lower capacity means
       * higher depth, default is 1
       * */
      OcTree(std::shared_ptr<Point3> p, float width, float height, float depth,
          int capacity = 1);

      typedef OcTreeIterator iterator;
      friend class OcTreeIterator;

      /**
       * @brief Will insert point into OcTree at correct subnode if the point is
       * valid. Valid points are those within the boundary defined by
       * information passed t constructor.
       *
       * @param point - point to be inserted
       *
       * @return true if point was successfully added, false otherwise. if
       * false, if is likely the point is not in the boundary of the OcTree
       * */
      std::pair<bool,iterator> insert(Point3* point);
      /**
       * @brief Will remove point from OcTree if the point is in the tree,
       * If multiple copies of the same point are in the tree, only the first
       * point found will be removed.
       *
       * @param point - point to be removed
       *
       * @return true if point was successfully removed, false otherwise. if
       * false, it is likely that the point was never in the OcTree
       * */
      bool remove(Point3* point);
      /**
       * @brief Check if point exists in OcTree
       *
       * @param point - point to be checked
       *
       * @return true if point is found in tree, false otherwise
       * */
      std::pair<bool,iterator> contains(Point3* point) const;
      /**
       * @brief Find all points within boundary defined by parameters
       *
       * @param point - center of boundary
       * @param width - total width of boundary
       * @param height - total height of boundary
       * @param depth - total depth of boundary
       *
       * @return a std::vector of Point3. Each Point3 is a copy of a point
       * within the tree
       * */
      std::vector<Point3> query(Point3* point, float width, float height, 
          float depth) const;

      /**
       * @brief Returns number of nodes in OcTree
       * */
      int size() const {return size_;};
      /**
       * @brief Removes all point from tree
       * */
      void clear();

    private:
      /*
       * recursively finds/creates correct node to insert point and inserts the
       * point there, starts at node passed as argument
       * */
      std::pair<bool,iterator> add_point(std::shared_ptr<OcTreeNode> node, Point3* point);
      /*
       * recursively finds a point and removes it
       * */
      bool remove_point(std::shared_ptr<OcTreeNode> node, Point3* point);
      /*
       * subdivides node, creating all proper children
       * */
      void subdivide(std::shared_ptr<OcTreeNode> node);
      /*
       * recursively search tree for a specific point, starts at given node
       * */
      std::pair<bool,iterator> search(std::shared_ptr<OcTreeNode> node, Point3* point) const;
      /*
       * recursively finds all points within given boundary and adds copies of
       * those points to the given vector
       * */
      void search_tree(
          std::vector<Point3>* points, std::shared_ptr<OcTreeNode> node, 
          Point3* center, float width, float height, float depth) const;
      
      std::shared_ptr<OcTreeNode> node_;
      int capacity_;
      int size_;
  };
}

#endif
