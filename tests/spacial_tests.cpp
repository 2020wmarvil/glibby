#include "glibby/primitives/point.h"
#include "glibby/primitives/point2D.h"
#include "glibby/spacial/OcTree.h"
#include "glibby/spacial/QuadTree.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <iostream>
#include <memory>
#include <random>
#include <vector>

/*
 * QUADTREE TESTS
 * */
TEST_CASE("QuadTree insert point", "[spacial][QuadTree]")
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.49f;
  pt4->y = -1.49f;
  CHECK(qt1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)).first);
}

TEST_CASE("QuadTree remove point", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.49f;
  pt4->y = -1.49f;
  CHECK(qt1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)).first);

  // remove pt2 and check pt3 and pt4 still exist
  CHECK(qt1.remove(&(*pt2)));
  CHECK(qt1.contains(&(*pt3)).first);
  CHECK(qt1.contains(&(*pt4)).first);

  CHECK(qt1.remove(&(*pt3)));
  CHECK(qt1.remove(&(*pt4)));
  CHECK_FALSE(qt1.remove(&(*pt5)));
  CHECK_FALSE(qt1.remove(&(*pt6)));
}

TEST_CASE("QuadTree contains point", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.45f;
  pt4->y = -1.45f;
  CHECK(qt1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)).first);

  CHECK(qt1.contains(&(*pt2)).first);
  CHECK(qt1.contains(&(*pt3)).first);
  CHECK(qt1.contains(&(*pt4)).first);
  CHECK_FALSE(qt1.contains(&(*pt5)).first);
  CHECK_FALSE(qt1.contains(&(*pt6)).first);
}

TEST_CASE("QuadTree random insertion and check", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::vector<std::shared_ptr<glibby::Point2D>> valid;
  std::vector<std::shared_ptr<glibby::Point2D>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) {
    std::shared_ptr<glibby::Point2D> temp(new glibby::Point2D);
    temp->x = distribution(gen);
    temp->y = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) {
    CHECK(qt1.insert(&(*(valid[i]))).first);
    CHECK(qt1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) {
    std::shared_ptr<glibby::Point2D> temp1(new glibby::Point2D);
    temp1->x = distribution(gen) + 3;
    temp1->y = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point2D> temp2(new glibby::Point2D);
    temp2->x = distribution(gen) - 3;
    temp2->y = distribution(gen) + 3;
    invalid.push_back(temp2);

    std::shared_ptr<glibby::Point2D> temp3(new glibby::Point2D);
    temp3->x = distribution(gen) + 3;
    temp3->y = distribution(gen) - 3;
    invalid.push_back(temp3);

    std::shared_ptr<glibby::Point2D> temp4(new glibby::Point2D);
    temp4->x = distribution(gen) - 3;
    temp4->y = distribution(gen) - 3;
    invalid.push_back(temp4);
  }
  for (int i = 0; i < invalid.size(); i++) {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("QuadTree larger capacity per node", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f, 5);

  std::vector<std::shared_ptr<glibby::Point2D>> valid;
  std::vector<std::shared_ptr<glibby::Point2D>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) {
    std::shared_ptr<glibby::Point2D> temp(new glibby::Point2D);
    temp->x = distribution(gen);
    temp->y = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) {
    CHECK(qt1.insert(&(*(valid[i]))).first);
    CHECK(qt1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) {
    std::shared_ptr<glibby::Point2D> temp1(new glibby::Point2D);
    temp1->x = distribution(gen) + 3;
    temp1->y = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point2D> temp2(new glibby::Point2D);
    temp2->x = distribution(gen) - 3;
    temp2->y = distribution(gen) + 3;
    invalid.push_back(temp2);

    std::shared_ptr<glibby::Point2D> temp3(new glibby::Point2D);
    temp3->x = distribution(gen) + 3;
    temp3->y = distribution(gen) - 3;
    invalid.push_back(temp3);

    std::shared_ptr<glibby::Point2D> temp4(new glibby::Point2D);
    temp4->x = distribution(gen) - 3;
    temp4->y = distribution(gen) - 3;
    invalid.push_back(temp4);
  }
  for (int i = 0; i < invalid.size(); i++) {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("QuadTree find all points in area", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::vector<glibby::Point2D> should_include;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      glibby::Point2D temp;
      temp.x = i;
      temp.y = j;
      should_include.push_back(temp);
      CHECK(qt1.insert(&temp).first);
      CHECK(qt1.contains(&temp).first);
    }
  }

  std::vector<glibby::Point2D> in_area = qt1.query(&(*pt1), 3.00f, 3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) {
      if (should_include[i].x == in_area[j].x &&
          should_include[i].y == in_area[j].y) {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("QuadTree find all randomized points in area",
          "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 5.00f, 5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2D> all_points;
  for (int i = 0; i < 100; i++) {
    glibby::Point2D temp;
    temp.x = distribution(gen);
    temp.y = distribution(gen);
    all_points.push_back(temp);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  float width = 2 * fabs(distribution(gen));
  float height = 2 * fabs(distribution(gen));

  std::vector<glibby::Point2D> in_area = qt1.query(&(*pt1), width, height);
  std::vector<glibby::Point2D> should_include;

  for (int i = 0; i < all_points.size(); i++) {
    if (all_points[i].x < width / 2 && all_points[i].x > -1 * width / 2 &&
        all_points[i].y < height / 2 && all_points[i].y > -1 * height / 2) {
      should_include.push_back(all_points[i]);
    }
  }

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) {
      if (should_include[i].x == in_area[j].x &&
          should_include[i].y == in_area[j].y) {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("QuadTree iterator testing", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 8.00f, 8.00f);

  std::vector<glibby::Point2D> all_points;
  std::vector<bool> found;

  float points_x[21] = {0,  -2, -2, 2, 2, -3, -3, -1, -1, -3, -3,
                        -1, -1, 1,  1, 3, 3,  1,  1,  3,  3};
  float points_y[21] = {0, -2, 2, 2, -2, -3, -1, -1, -3, 1, 3,
                        3, 1,  1, 3, 3,  1,  -3, -1, -1, -3};

  glibby::Point2D temp;
  for (int i = 0; i < 21; i++) {
    temp.x = points_x[i];
    temp.y = points_y[i];
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  glibby::QuadTree::iterator itr = qt1.begin();
  while (itr != qt1.end()) {
    std::shared_ptr<const glibby::Point2D> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->x - all_points[i].x) < 0.001f &&
          fabs(temp->y - all_points[i].y) < 0.001f) {
        if (found[i]) { // this point has already been marked by a different
                        // point in tree
          continue;
        }
        contains = true;
        found[i] = true;
        break;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }
}

TEST_CASE("QuadTree random iterator testing", "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2D> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 25; i++) {
    glibby::Point2D temp;
    temp.x = distribution(gen);
    temp.y = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }
  for (int i = 0; i < 25; i++) {
    std::cout << all_points[i].x << " " << all_points[i].y << std::endl;
  }
  std::cout << std::endl << std::endl;

  glibby::QuadTree::iterator itr = qt1.begin();
  while (itr != qt1.end()) {
    std::shared_ptr<const glibby::Point2D> temp = *itr;
    std::cout << temp->x << " " << temp->y << std::endl;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->x - all_points[i].x) < 0.001f &&
          fabs(temp->y - all_points[i].y) < 0.001f) {
        if (found[i]) { // this point has already been marked by a different
                        // point in tree
          continue;
        }
        contains = true;
        found[i] = true;
        break;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }
}

TEST_CASE("QuadTree iterator testing with insertion and removal",
          "[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2D> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 25; i++) {
    glibby::Point2D temp;
    temp.x = distribution(gen);
    temp.y = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  glibby::QuadTree::iterator itr = qt1.begin();
  while (itr != qt1.end()) {
    std::shared_ptr<const glibby::Point2D> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->x - all_points[i].x) < 0.001f &&
          fabs(temp->y - all_points[i].y) < 0.001f) {
        if (found[i]) { // this point has already been marked by a different
                        // point in tree
          continue;
        }
        contains = true;
        found[i] = true;
        break;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }

  // remove every other point
  for (unsigned int i = 0; i < all_points.size(); i += 2) {
    qt1.remove(&all_points[i]);
  }
  // reset everything and check again
  for (unsigned int i = 0; i < found.size(); i++) {
    found[i] = false;
  }
  itr = qt1.begin();
  while (itr != qt1.end()) {
    std::shared_ptr<const glibby::Point2D> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->x - all_points[i].x) < 0.001f &&
          fabs(temp->y - all_points[i].y) < 0.001f) {
        if (found[i]) { // this point has already been marked by a different
                        // point in tree
          continue;
        }
        contains = true;
        found[i] = true;
        break;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }

  // add all the removed points back
  for (unsigned int i = 0; i < all_points.size(); i += 2) {
    qt1.insert(&all_points[i]);
  }
  // reset everything and check again
  for (unsigned int i = 0; i < found.size(); i++) {
    found[i] = false;
  }
  itr = qt1.begin();
  while (itr != qt1.end()) {
    std::shared_ptr<const glibby::Point2D> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->x - all_points[i].x) < 0.001f &&
          fabs(temp->y - all_points[i].y) < 0.001f) {
        if (found[i]) { // this point has already been marked by a different
                        // point in tree
          continue;
        }
        contains = true;
        found[i] = true;
        break;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }
}

/*
 * OCTREE TESTS
 * */
TEST_CASE("OcTree insert point", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->points[0] = 1.00f;
  pt2->points[1] = 1.00f;
  pt2->points[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->points[0] = 1.00f;
  pt3->points[1] = 1.00f;
  pt3->points[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->points[0] = 1.49f;
  pt4->points[1] = -1.49f;
  pt4->points[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->points[0] = 3.00f;
  pt5->points[1] = -3.00f;
  pt5->points[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->points[0] = -10.00f;
  pt6->points[1] = 10.00f;
  pt6->points[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)).first);
}

TEST_CASE("OcTree remove point", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->points[0] = 1.00f;
  pt2->points[1] = 1.00f;
  pt2->points[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->points[0] = 1.00f;
  pt3->points[1] = 1.00f;
  pt3->points[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->points[0] = 1.49f;
  pt4->points[1] = -1.49f;
  pt4->points[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->points[0] = 3.00f;
  pt5->points[1] = -3.00f;
  pt5->points[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->points[0] = -10.00f;
  pt6->points[1] = 10.00f;
  pt6->points[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)).first);

  // remove pt2 and check pt3 and pt4 still exist
  CHECK(ot1.remove(&(*pt2)));
  CHECK(ot1.contains(&(*pt3)).first);
  CHECK(ot1.contains(&(*pt4)).first);

  CHECK(ot1.remove(&(*pt3)));
  CHECK(ot1.remove(&(*pt4)));
  CHECK_FALSE(ot1.remove(&(*pt5)));
  CHECK_FALSE(ot1.remove(&(*pt6)));
}

TEST_CASE("OcTree contains point", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->points[0] = 1.00f;
  pt2->points[1] = 1.00f;
  pt2->points[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->points[0] = 1.00f;
  pt3->points[1] = 1.00f;
  pt3->points[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->points[0] = 1.49f;
  pt4->points[1] = -1.49f;
  pt4->points[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->points[0] = 3.00f;
  pt5->points[1] = -3.00f;
  pt5->points[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->points[0] = -10.00f;
  pt6->points[1] = 10.00f;
  pt6->points[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)).first);

  CHECK(ot1.contains(&(*pt2)).first);
  CHECK(ot1.contains(&(*pt3)).first);
  CHECK(ot1.contains(&(*pt4)).first);
  CHECK_FALSE(ot1.contains(&(*pt5)).first);
  CHECK_FALSE(ot1.contains(&(*pt6)).first);
}

TEST_CASE("OcTree random insertion and check", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::vector<std::shared_ptr<glibby::Point3>> valid;
  std::vector<std::shared_ptr<glibby::Point3>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) {
    std::shared_ptr<glibby::Point3> temp(new glibby::Point3);
    temp->points[0] = distribution(gen);
    temp->points[1] = distribution(gen);
    temp->points[2] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) {
    CHECK(ot1.insert(&(*(valid[i]))).first);
    CHECK(ot1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) {
    std::shared_ptr<glibby::Point3> temp1(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp2(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp3(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp4(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp5(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp6(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp7(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp8(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);
  }
  for (int i = 0; i < invalid.size(); i++) {
    CHECK_FALSE(ot1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(ot1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("OcTree random insertion with larger capacity", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f, 5);

  std::vector<std::shared_ptr<glibby::Point3>> valid;
  std::vector<std::shared_ptr<glibby::Point3>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) {
    std::shared_ptr<glibby::Point3> temp(new glibby::Point3);
    temp->points[0] = distribution(gen);
    temp->points[1] = distribution(gen);
    temp->points[2] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) {
    CHECK(ot1.insert(&(*(valid[i]))).first);
    CHECK(ot1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) {
    std::shared_ptr<glibby::Point3> temp1(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp2(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp3(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp4(new glibby::Point3);
    temp1->points[0] = distribution(gen) + 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp5(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp6(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) + 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp7(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp8(new glibby::Point3);
    temp1->points[0] = distribution(gen) - 3;
    temp1->points[1] = distribution(gen) - 3;
    temp1->points[2] = distribution(gen) - 3;
    invalid.push_back(temp1);
  }
  for (int i = 0; i < invalid.size(); i++) {
    CHECK_FALSE(ot1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(ot1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("OcTree find all points in area", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::vector<glibby::Point3> should_include;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      for (int k = -1; k <= 1; k++) {
        glibby::Point3 temp;
        temp.points[0] = i;
        temp.points[1] = j;
        temp.points[2] = k;
        should_include.push_back(temp);
        CHECK(ot1.insert(&temp).first);
        CHECK(ot1.contains(&temp).first);
      }
    }
  }

  std::vector<glibby::Point3> in_area = ot1.query(&(*pt1), 3.00f, 3.00f, 3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) {
      if (should_include[i].points[0] == in_area[j].points[0] &&
          should_include[i].points[1] == in_area[j].points[1] &&
          should_include[i].points[2] == in_area[j].points[2]) {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("OcTree find all points in randomized area", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 5.00f, 5.00f, 5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point3> all_points;
  for (int i = 0; i < 1000; i++) {
    glibby::Point3 temp;
    temp.points[0] = distribution(gen);
    temp.points[1] = distribution(gen);
    temp.points[2] = distribution(gen);
    all_points.push_back(temp);
    CHECK(ot1.insert(&temp).first);
    CHECK(ot1.contains(&temp).first);
  }

  float width = 2 * fabs(distribution(gen));
  float height = 2 * fabs(distribution(gen));
  float depth = 2 * fabs(distribution(gen));

  std::vector<glibby::Point3> in_area =
      ot1.query(&(*pt1), width, height, depth);
  std::vector<glibby::Point3> should_include;

  for (int i = 0; i < all_points.size(); i++) {
    if (all_points[i].points[0] < width / 2 &&
        all_points[i].points[0] > -1 * width / 2 &&
        all_points[i].points[1] < height / 2 &&
        all_points[i].points[1] > -1 * height / 2 &&
        all_points[i].points[2] < depth / 2 &&
        all_points[i].points[2] > -1 * depth / 2) {
      should_include.push_back(all_points[i]);
    }
  }

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) {
      if (should_include[i].points[0] == in_area[j].points[0] &&
          should_include[i].points[1] == in_area[j].points[1] &&
          should_include[i].points[2] == in_area[j].points[2]) {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("OcTree iterator testing", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 8.00f, 8.00f, 8.00f);

  // START HERE

  std::vector<glibby::Point3> all_points;
  std::vector<bool> found;

  float points_x[25] = {0,  -2, -2, 2,  2, -2, -2, 2, 2, -3, -3, -1, -1,
                        -3, -3, -1, -1, 1, 1,  3,  3, 1, 1,  3,  3};
  float points_y[25] = {0, -2, 2, 2, -2, -2, 2, -2, 2,  -3, -1, -1, -3,
                        1, 3,  3, 1, 1,  3,  3, 1,  -3, -1, -1, -3};
  float points_z[25] = {0, -2, -2, -2, -2, 2, 2, 2, 2,  -3, -3, -1, -1,
                        3, 3,  1,  1,  3,  3, 1, 1, -3, -3, -1, -1};

  glibby::Point3 temp;
  for (int i = 0; i < 25; i++) {
    temp.points[0] = points_x[i];
    temp.points[1] = points_y[i];
    temp.points[2] = points_z[i];
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(ot1.insert(&temp).first);
    CHECK(ot1.contains(&temp).first);
  }

  glibby::OcTree::iterator itr = ot1.begin();
  while (itr != ot1.end()) {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->points[0] - all_points[i].points[0]) < 0.001f &&
          fabs(temp->points[1] - all_points[i].points[1]) < 0.001f &&
          fabs(temp->points[2] - all_points[i].points[2]) < 0.001f) {
        contains = true;
        found[i] = true;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }
}

TEST_CASE("OcTree random iterator testing", "[spacial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->points[0] = 0.00f;
  pt1->points[1] = 0.00f;
  pt1->points[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 5.00f, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point3> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 100; i++) {
    glibby::Point3 temp;
    temp.points[0] = distribution(gen);
    temp.points[1] = distribution(gen);
    temp.points[2] = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(ot1.insert(&temp).first);
    CHECK(ot1.contains(&temp).first);
  }

  glibby::OcTree::iterator itr = ot1.begin();
  while (itr != ot1.end()) {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) {
      if (fabs(temp->points[0] - all_points[i].points[0]) < 0.001f &&
          fabs(temp->points[1] - all_points[i].points[1]) < 0.001f &&
          fabs(temp->points[2] - all_points[i].points[2]) < 0.001f) {
        contains = true;
        found[i] = true;
      }
    }
    CHECK(contains);

    itr++;
  }
  for (unsigned int i = 0; i < found.size(); i++) {
    CHECK(found[i]);
  }
}
