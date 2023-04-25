#include "glibby/spatial/kdTree.h"
#include "glibby/spatial/OcTree.h"
#include "glibby/spatial/QuadTree.h"
#include "glibby/primitives/point.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <memory>
#include <random>
#include <vector>

TEST_CASE("kdTree Printed", "[spatial][kdTree]") {
        glibby::Point3 p1;
        glibby::Point3 p2({ 5.0f, 15.0f, 10.0f });
        glibby::Point3 p3({ 11.0f, 16.0f, 8.0f});
        glibby::Point3 p4({ 6.0f, 8.0f, 9.0f });
        glibby::Point3 p5({ 21.0f, 19.0f, 78.0f });
        glibby::kdTree<float, 3> KT({p1, p2, p3, p4, p5});
        glibby::Point3 p6({ 21.0f, 18.0f, 78.0f });
        KT.DFSPrint();
        KT.Insert(p6);
        KT.DFSPrint();
        std::vector<glibby::Point3> test = {p6, p2};
        KT.Delete(test);
        KT.DFSPrint();
}

/*
 * QUADTREE TESTS
 * */
TEST_CASE("QuadTree insert point", "[spatial][QuadTree]")
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point2> pt2(new glibby::Point2);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  CHECK(qt1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point2> pt3(new glibby::Point2);
  pt3->coord[0] = -1.00f;
  pt3->coord[1] = -1.00f;
  CHECK(qt1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point2> pt4(new glibby::Point2);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  CHECK(qt1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point2> pt5(new glibby::Point2);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point2> pt6(new glibby::Point2);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)).first);
}

TEST_CASE("QuadTree remove point", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point2> pt2(new glibby::Point2);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  CHECK(qt1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point2> pt3(new glibby::Point2);
  pt3->coord[0] = -1.00f;
  pt3->coord[1] = -1.00f;
  CHECK(qt1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point2> pt4(new glibby::Point2);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  CHECK(qt1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point2> pt5(new glibby::Point2);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point2> pt6(new glibby::Point2);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = -10.00f;
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

TEST_CASE("QuadTree contains point", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point2> pt2(new glibby::Point2);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  CHECK(qt1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point2> pt3(new glibby::Point2);
  pt3->coord[0] = -1.00f;
  pt3->coord[1] = -1.00f;
  CHECK(qt1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point2> pt4(new glibby::Point2);
  pt4->coord[0] = 1.45f;
  pt4->coord[1] = -1.45f;
  CHECK(qt1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point2> pt5(new glibby::Point2);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point2> pt6(new glibby::Point2);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)).first);

  CHECK(qt1.contains(&(*pt2)).first);
  CHECK(qt1.contains(&(*pt3)).first);
  CHECK(qt1.contains(&(*pt4)).first);
  CHECK_FALSE(qt1.contains(&(*pt5)).first);
  CHECK_FALSE(qt1.contains(&(*pt6)).first);
}

TEST_CASE("QuadTree random insertion and check", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::vector<std::shared_ptr<glibby::Point2>> valid;
  std::vector<std::shared_ptr<glibby::Point2>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point2> temp(new glibby::Point2);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) 
  {
    CHECK(qt1.insert(&(*(valid[i]))).first);
    CHECK(qt1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) 
  {
    std::shared_ptr<glibby::Point2> temp1(new glibby::Point2);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point2> temp2(new glibby::Point2);
    temp2->coord[0] = distribution(gen) - 3;
    temp2->coord[1] = distribution(gen) + 3;
    invalid.push_back(temp2);

    std::shared_ptr<glibby::Point2> temp3(new glibby::Point2);
    temp3->coord[0] = distribution(gen) + 3;
    temp3->coord[1] = distribution(gen) - 3;
    invalid.push_back(temp3);

    std::shared_ptr<glibby::Point2> temp4(new glibby::Point2);
    temp4->coord[0] = distribution(gen) - 3;
    temp4->coord[1] = distribution(gen) - 3;
    invalid.push_back(temp4);
  }
  for (int i = 0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("QuadTree larger capacity per node", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f, 5);

  std::vector<std::shared_ptr<glibby::Point2>> valid;
  std::vector<std::shared_ptr<glibby::Point2>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point2> temp(new glibby::Point2);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) 
  {
    CHECK(qt1.insert(&(*(valid[i]))).first);
    CHECK(qt1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) 
  {
    std::shared_ptr<glibby::Point2> temp1(new glibby::Point2);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point2> temp2(new glibby::Point2);
    temp2->coord[0] = distribution(gen) - 3;
    temp2->coord[1] = distribution(gen) + 3;
    invalid.push_back(temp2);

    std::shared_ptr<glibby::Point2> temp3(new glibby::Point2);
    temp3->coord[0] = distribution(gen) + 3;
    temp3->coord[1] = distribution(gen) - 3;
    invalid.push_back(temp3);

    std::shared_ptr<glibby::Point2> temp4(new glibby::Point2);
    temp4->coord[0] = distribution(gen) - 3;
    temp4->coord[1] = distribution(gen) - 3;
    invalid.push_back(temp4);
  }
  for (int i = 0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("QuadTree find all points in area", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 3.00f, 3.00f);

  std::vector<glibby::Point2> should_include;
  for (int i = -1; i <= 1; i++) 
  {
    for (int j = -1; j <= 1; j++) 
    {
      glibby::Point2 temp;
      temp.coord[0] = i;
      temp.coord[1] = j;
      should_include.push_back(temp);
      CHECK(qt1.insert(&temp).first);
      CHECK(qt1.contains(&temp).first);
    }
  }

  std::vector<glibby::Point2> in_area = qt1.query(&(*pt1), 3.00f, 3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) 
    {
      if (should_include[i].coord[0] == in_area[j].coord[0] &&
          should_include[i].coord[1] == in_area[j].coord[1])
      {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("QuadTree find all randomized points in area",
          "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 5.00f, 5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2> all_points;
  for (int i = 0; i < 100; i++) 
  {
    glibby::Point2 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    all_points.push_back(temp);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  float width = 2 * fabs(distribution(gen));
  float height = 2 * fabs(distribution(gen));

  std::vector<glibby::Point2> in_area = qt1.query(&(*pt1), width, height);
  std::vector<glibby::Point2> should_include;

  for (int i = 0; i < all_points.size(); i++) 
  {
    if (all_points[i].coord[0] < width / 2 && all_points[i].coord[0] > -1 * width / 2 &&
        all_points[i].coord[1] < height / 2 && all_points[i].coord[1] > -1 * height / 2) 
    {
      should_include.push_back(all_points[i]);
    }
  }

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) 
    {
      if (should_include[i].coord[0] == in_area[j].coord[0] &&
          should_include[i].coord[1] == in_area[j].coord[1])
      {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("QuadTree iterator testing", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 8.00f, 8.00f);

  std::vector<glibby::Point2> all_points;
  std::vector<bool> found;

  float points_x[21] = {0,  -2, -2, 2, 2, -3, -3, -1, -1, -3, -3,
                        -1, -1, 1,  1, 3, 3,  1,  1,  3,  3};
  float points_y[21] = {0, -2, 2, 2, -2, -3, -1, -1, -3, 1, 3,
                        3, 1,  1, 3, 3,  1,  -3, -1, -1, -3};

  glibby::Point2 temp;
  for (int i = 0; i < 21; i++) 
  {
    temp.coord[0] = points_x[i];
    temp.coord[1] = points_y[i];
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  glibby::QuadTree::iterator itr = qt1.begin();
  while (itr != qt1.end()) 
  {
    std::shared_ptr<const glibby::Point2> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f) 
      {
        if (found[i]) 
        { // this point has already been marked by a different
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
}

TEST_CASE("QuadTree random iterator testing", "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 25; i++) 
  {
    glibby::Point2 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  glibby::QuadTree::iterator itr = qt1.begin();
  while (itr != qt1.end()) 
  {
    std::shared_ptr<const glibby::Point2> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f) 
      {
        if (found[i]) 
        { // this point has already been marked by a different
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
}

TEST_CASE("QuadTree iterator testing with insertion and removal",
          "[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 25; i++) 
  {
    glibby::Point2 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(qt1.insert(&temp).first);
    CHECK(qt1.contains(&temp).first);
  }

  glibby::QuadTree::iterator itr = qt1.begin();
  while (itr != qt1.end()) 
  {
    std::shared_ptr<const glibby::Point2> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f) 
      {
        if (found[i]) 
        { // this point has already been marked by a different
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }

  // remove every other point
  for (unsigned int i = 0; i < all_points.size(); i += 2) 
  {
    qt1.remove(&all_points[i]);
  }
  // reset everything and check again
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    found[i] = false;
  }
  itr = qt1.begin();
  while (itr != qt1.end()) 
  {
    std::shared_ptr<const glibby::Point2> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f) 
      {
        if (found[i]) 
        { // this point has already been marked by a different
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
  // all even points were removed, so only check the odd ones
  for (unsigned int i = 1; i < found.size(); i += 2) 
  {
    CHECK(found[i]);
  }

  // add all the removed points back
  for (unsigned int i = 0; i < all_points.size(); i += 2) 
  {
    qt1.insert(&all_points[i]);
  }
  // reset everything and check again
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    found[i] = false;
  }
  itr = qt1.begin();
  while (itr != qt1.end()) 
  {
    std::shared_ptr<const glibby::Point2> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f) 
      {
        if (found[i]) 
        { // this point has already been marked by a different
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
}

/*
 * OCTREE TESTS
 * */
TEST_CASE("OcTree insert point", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  pt2->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->coord[0] = 1.00f;
  pt3->coord[1] = 1.00f;
  pt3->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  pt4->coord[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  pt5->coord[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = 10.00f;
  pt6->coord[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)).first);
}

TEST_CASE("OcTree remove point", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  pt2->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->coord[0] = 1.00f;
  pt3->coord[1] = 1.00f;
  pt3->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  pt4->coord[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  pt5->coord[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = 10.00f;
  pt6->coord[2] = -10.00f;
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

TEST_CASE("OcTree contains point", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  pt2->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)).first);

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->coord[0] = 1.00f;
  pt3->coord[1] = 1.00f;
  pt3->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)).first);

  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  pt4->coord[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)).first);

  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  pt5->coord[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)).first);

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = 10.00f;
  pt6->coord[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)).first);

  CHECK(ot1.contains(&(*pt2)).first);
  CHECK(ot1.contains(&(*pt3)).first);
  CHECK(ot1.contains(&(*pt4)).first);
  CHECK_FALSE(ot1.contains(&(*pt5)).first);
  CHECK_FALSE(ot1.contains(&(*pt6)).first);
}

TEST_CASE("OcTree random insertion and check", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::vector<std::shared_ptr<glibby::Point3>> valid;
  std::vector<std::shared_ptr<glibby::Point3>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point3> temp(new glibby::Point3);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    temp->coord[2] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) 
  {
    CHECK(ot1.insert(&(*(valid[i]))).first);
    CHECK(ot1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) 
  {
    std::shared_ptr<glibby::Point3> temp1(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp2(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp3(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp4(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp5(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp6(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp7(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp8(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);
  }
  for (int i = 0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(ot1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(ot1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("OcTree random insertion with larger capacity", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f, 5);

  std::vector<std::shared_ptr<glibby::Point3>> valid;
  std::vector<std::shared_ptr<glibby::Point3>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-1.5f, 1.5f);

  for (int i = 0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point3> temp(new glibby::Point3);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    temp->coord[2] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i = 0; i < valid.size(); i++) 
  {
    CHECK(ot1.insert(&(*(valid[i]))).first);
    CHECK(ot1.contains(&(*(valid[i]))).first);
  }

  for (int i = 0; i < 25; i++) 
  {
    std::shared_ptr<glibby::Point3> temp1(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp2(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp3(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp4(new glibby::Point3);
    temp1->coord[0] = distribution(gen) + 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp5(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp6(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) + 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp7(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) + 3;
    invalid.push_back(temp1);

    std::shared_ptr<glibby::Point3> temp8(new glibby::Point3);
    temp1->coord[0] = distribution(gen) - 3;
    temp1->coord[1] = distribution(gen) - 3;
    temp1->coord[2] = distribution(gen) - 3;
    invalid.push_back(temp1);
  }
  for (int i = 0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(ot1.insert(&(*(invalid[i]))).first);
    CHECK_FALSE(ot1.contains(&(*(invalid[i]))).first);
  }
}

TEST_CASE("OcTree find all points in area", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 3.00f, 3.00f, 3.00f);

  std::vector<glibby::Point3> should_include;
  for (int i = -1; i <= 1; i++) 
  {
    for (int j = -1; j <= 1; j++) 
    {
      for (int k = -1; k <= 1; k++) 
      {
        glibby::Point3 temp;
        temp.coord[0] = i;
        temp.coord[1] = j;
        temp.coord[2] = k;
        should_include.push_back(temp);
        CHECK(ot1.insert(&temp).first);
        CHECK(ot1.contains(&temp).first);
      }
    }
  }

  std::vector<glibby::Point3> in_area = ot1.query(&(*pt1), 3.00f, 3.00f, 3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) 
    {
      if (should_include[i].coord[0] == in_area[j].coord[0] &&
          should_include[i].coord[1] == in_area[j].coord[1] &&
          should_include[i].coord[2] == in_area[j].coord[2]) 
      {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("OcTree find all points in randomized area", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 5.00f, 5.00f, 5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point3> all_points;
  for (int i = 0; i < 1000; i++) 
  {
    glibby::Point3 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    temp.coord[2] = distribution(gen);
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

  for (int i = 0; i < all_points.size(); i++) 
  {
    if (all_points[i].coord[0] < width / 2 &&
        all_points[i].coord[0] > -1 * width / 2 &&
        all_points[i].coord[1] < height / 2 &&
        all_points[i].coord[1] > -1 * height / 2 &&
        all_points[i].coord[2] < depth / 2 &&
        all_points[i].coord[2] > -1 * depth / 2) 
    {
      should_include.push_back(all_points[i]);
    }
  }

  CHECK(in_area.size() == should_include.size());

  for (int i = 0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j = 0; j < in_area.size(); j++) 
    {
      if (should_include[i].coord[0] == in_area[j].coord[0] &&
          should_include[i].coord[1] == in_area[j].coord[1] &&
          should_include[i].coord[2] == in_area[j].coord[2]) 
      {
        in = true;
        break;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("OcTree iterator testing", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
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
  for (int i = 0; i < 25; i++) 
  {
    temp.coord[0] = points_x[i];
    temp.coord[1] = points_y[i];
    temp.coord[2] = points_z[i];
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(ot1.insert(&temp).first);
    CHECK(ot1.contains(&temp).first);
  }

  glibby::OcTree::iterator itr = ot1.begin();
  while (itr != ot1.end()) 
  {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f &&
          fabs(temp->coord[2] - all_points[i].coord[2]) < 0.001f) 
      {
        if (found[i])
        { // this point has already been marked by a different
          // poin in tree
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
}

TEST_CASE("OcTree random iterator testing", "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 5.00f, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point3> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 100; i++) 
  {
    glibby::Point3 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    temp.coord[2] = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(ot1.insert(&temp).first);
    CHECK(ot1.contains(&temp).first);
  }

  glibby::OcTree::iterator itr = ot1.begin();
  while (itr != ot1.end()) 
  {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f &&
          fabs(temp->coord[2] - all_points[i].coord[2]) < 0.001f) 
      {
        if (found[i])
        { // this point has already been marked by a different poin in tree
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
}

TEST_CASE("OcTree random iterator testing with insertion and removal", 
    "[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1, 5.00f, 5.00f, 5.00f, 3);

  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(-2.5f, 2.5f);

  std::vector<glibby::Point3> all_points;
  std::vector<bool> found;
  for (int i = 0; i < 100; i++) 
  {
    glibby::Point3 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    temp.coord[2] = distribution(gen);
    all_points.push_back(temp);
    found.push_back(false);
    CHECK(ot1.insert(&temp).first);
    CHECK(ot1.contains(&temp).first);
  }

  glibby::OcTree::iterator itr = ot1.begin();
  while (itr != ot1.end()) 
  {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f &&
          fabs(temp->coord[2] - all_points[i].coord[2]) < 0.001f) 
      {
        if (found[i])
        { // this point has already been marked by a different poin in tree
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
  // remove every other point
  for (unsigned int i=0; i < all_points.size(); i += 2)
  {
    ot1.remove(&all_points[i]);
  }
  // reset everything and check again
  for (unsigned int i=0; i < found.size(); i++)
  {
    found[i] = false;
  }
  itr = ot1.begin();
  while (itr != ot1.end()) 
  {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f &&
          fabs(temp->coord[2] - all_points[i].coord[2]) < 0.001f) 
      {
        if (found[i])
        { // this point has already been marked by a different poin in tree
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
  for (unsigned int i = 1; i < found.size(); i += 2) 
  {
    CHECK(found[i]);
  }
  // add every removed point back
  for (unsigned int i=0; i < all_points.size(); i += 2)
  {
    ot1.insert(&all_points[i]);
  }
  // reset everything and check again
  for (unsigned int i=0; i < found.size(); i++)
  {
    found[i] = false;
  }
  itr = ot1.begin();
  while (itr != ot1.end()) 
  {
    std::shared_ptr<const glibby::Point3> temp = *itr;
    bool contains = false;
    for (int i = 0; i < all_points.size(); i++) 
    {
      if (fabs(temp->coord[0] - all_points[i].coord[0]) < 0.001f &&
          fabs(temp->coord[1] - all_points[i].coord[1]) < 0.001f &&
          fabs(temp->coord[2] - all_points[i].coord[2]) < 0.001f) 
      {
        if (found[i])
        { // this point has already been marked by a different poin in tree
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
  for (unsigned int i = 0; i < found.size(); i++) 
  {
    CHECK(found[i]);
  }
}
