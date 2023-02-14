#include "glibby/primitives/point2D.h"
#include "glibby/spacial/QuadTree.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>

TEST_CASE("QuadTree insert point","[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(pt2));

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(pt3));
  
  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.49f;
  pt4->y = -1.49f;
  CHECK(qt1.insert(pt4));
  
  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(pt5));

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(pt6));
}

TEST_CASE("QuadTree contains point","[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(pt2));

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(pt3));
  
  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.45f;
  pt4->y = -1.45f;
  CHECK(qt1.insert(pt4));
  
  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(pt5));

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(pt6));

  CHECK(qt1.contains(pt2));
  CHECK(qt1.contains(pt3));
  CHECK(qt1.contains(pt4));
  CHECK_FALSE(qt1.contains(pt5));
  CHECK_FALSE(qt1.contains(pt6));
}

