#include "glibby/primitives/point.h"
#include "glibby/spatial/QuadTree.h"
#include "glibby/primitives/point.h"
#include "glibby/spatial/OcTree.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>
#include <vector>
#include <random>
#include <iostream>

/*
 * QUADTREE TESTS
 * */
TEST_CASE("QuadTree insert point","[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2> pt2(new glibby::Point2);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  CHECK(qt1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point2> pt3(new glibby::Point2);
  pt3->coord[0] = -1.00f;
  pt3->coord[1] = -1.00f;
  CHECK(qt1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point2> pt4(new glibby::Point2);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  CHECK(qt1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point2> pt5(new glibby::Point2);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point2> pt6(new glibby::Point2);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)));
}

TEST_CASE("QuadTree remove point","[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2> pt2(new glibby::Point2);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  CHECK(qt1.insert(&(*pt2)));
  CHECK(qt1.remove(&(*pt2)));

  std::shared_ptr<glibby::Point2> pt3(new glibby::Point2);
  pt3->coord[0] = -1.00f;
  pt3->coord[1] = -1.00f;
  CHECK(qt1.insert(&(*pt3)));
  CHECK(qt1.remove(&(*pt3)));
  
  std::shared_ptr<glibby::Point2> pt4(new glibby::Point2);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  CHECK(qt1.insert(&(*pt4)));
  CHECK(qt1.remove(&(*pt4)));
  
  std::shared_ptr<glibby::Point2> pt5(new glibby::Point2);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)));
  CHECK_FALSE(qt1.remove(&(*pt5)));

  std::shared_ptr<glibby::Point2> pt6(new glibby::Point2);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)));
  CHECK_FALSE(qt1.remove(&(*pt6)));
}


TEST_CASE("QuadTree contains point","[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2> pt2(new glibby::Point2);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  CHECK(qt1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point2> pt3(new glibby::Point2);
  pt3->coord[0] = -1.00f;
  pt3->coord[1] = -1.00f;
  CHECK(qt1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point2> pt4(new glibby::Point2);
  pt4->coord[0] = 1.45f;
  pt4->coord[1] = -1.45f;
  CHECK(qt1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point2> pt5(new glibby::Point2);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point2> pt6(new glibby::Point2);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)));

  CHECK(qt1.contains(&(*pt2)));
  CHECK(qt1.contains(&(*pt3)));
  CHECK(qt1.contains(&(*pt4)));
  CHECK_FALSE(qt1.contains(&(*pt5)));
  CHECK_FALSE(qt1.contains(&(*pt6)));
}

TEST_CASE("QuadTree random insertion and check","[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f);

  std::vector<std::shared_ptr<glibby::Point2>> valid;
  std::vector<std::shared_ptr<glibby::Point2>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-1.5f, 1.5f);

  for (int i=0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point2> temp(new glibby::Point2);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i=0; i < valid.size(); i++) 
  {
    CHECK(qt1.insert(&(*(valid[i]))));
    CHECK(qt1.contains(&(*(valid[i]))));
  }

  for (int i=0; i < 25; i++) 
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
  for (int i=0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))));
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))));
  }

}

TEST_CASE("QuadTree larger capacity per node","[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f,5);

  std::vector<std::shared_ptr<glibby::Point2>> valid;
  std::vector<std::shared_ptr<glibby::Point2>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-1.5f, 1.5f);

  for (int i=0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point2> temp(new glibby::Point2);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i=0; i < valid.size(); i++) 
  {
    CHECK(qt1.insert(&(*(valid[i]))));
    CHECK(qt1.contains(&(*(valid[i]))));
  }

  for (int i=0; i < 25; i++) 
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
  for (int i=0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))));
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))));
  }

}


TEST_CASE("QuadTree find all points in area","[spatial][QuadTree]") {
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f);

  std::vector<glibby::Point2> should_include;
  for (int i=-1; i <= 1; i++) 
  {
    for (int j=-1; j <= 1; j++) 
    {
      glibby::Point2 temp;
      temp.coord[0] = i;
      temp.coord[1] = j;
      should_include.push_back(temp);
      CHECK(qt1.insert(&temp));
      CHECK(qt1.contains(&temp));
    }
  }

  std::vector<glibby::Point2> in_area = qt1.query(&(*pt1),3.00f,3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i=0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j=0; j < in_area.size(); j++) 
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

TEST_CASE("QuadTree find all randomized points in area","[spatial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2> pt1(new glibby::Point2);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,5.00f,5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2> all_points;
  for (int i=0; i < 100; i++) 
  {
    glibby::Point2 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    all_points.push_back(temp);
    CHECK(qt1.insert(&temp));
    CHECK(qt1.contains(&temp));
  }

  float width = 2*fabs(distribution(gen));
  float height = 2*fabs(distribution(gen));

  std::vector<glibby::Point2> in_area = qt1.query(&(*pt1),width,height);
  std::vector<glibby::Point2> should_include;

  for (int i=0; i < all_points.size(); i++) 
  {
    if (all_points[i].coord[0] < width / 2 && all_points[i].coord[0] > -1 * width / 2 &&
        all_points[i].coord[1] < height / 2 && all_points[i].coord[1] > -1 * height / 2)
    {
      should_include.push_back(all_points[i]);
    }
  }

  CHECK(in_area.size() == should_include.size());

  for (int i=0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j=0; j < in_area.size(); j++) 
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

/*
 * OCTREE TESTS
 * */
TEST_CASE("OcTree insert point","[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,3.00f,3.00f,3.00f); 

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  pt2->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->coord[0] = 1.00f;
  pt3->coord[1] = 1.00f;
  pt3->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  pt4->coord[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  pt5->coord[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = 10.00f;
  pt6->coord[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)));
}

TEST_CASE("OcTree remove point","[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,3.00f,3.00f,3.00f); 

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  pt2->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->coord[0] = 1.00f;
  pt3->coord[1] = 1.00f;
  pt3->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  pt4->coord[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  pt5->coord[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = 10.00f;
  pt6->coord[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)));
  
  CHECK(ot1.remove(&(*pt2)));
  CHECK(ot1.remove(&(*pt3)));
  CHECK(ot1.remove(&(*pt4)));
  CHECK_FALSE(ot1.remove(&(*pt5)));
  CHECK_FALSE(ot1.remove(&(*pt6)));
}

TEST_CASE("OcTree contains point","[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,3.00f,3.00f,3.00f); 

  std::shared_ptr<glibby::Point3> pt2(new glibby::Point3);
  pt2->coord[0] = 1.00f;
  pt2->coord[1] = 1.00f;
  pt2->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point3> pt3(new glibby::Point3);
  pt3->coord[0] = 1.00f;
  pt3->coord[1] = 1.00f;
  pt3->coord[2] = 1.00f;
  CHECK(ot1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point3> pt4(new glibby::Point3);
  pt4->coord[0] = 1.49f;
  pt4->coord[1] = -1.49f;
  pt4->coord[2] = 1.49f;
  CHECK(ot1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point3> pt5(new glibby::Point3);
  pt5->coord[0] = 3.00f;
  pt5->coord[1] = -3.00f;
  pt5->coord[2] = 3.00f;
  CHECK_FALSE(ot1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point3> pt6(new glibby::Point3);
  pt6->coord[0] = -10.00f;
  pt6->coord[1] = 10.00f;
  pt6->coord[2] = -10.00f;
  CHECK_FALSE(ot1.insert(&(*pt6)));
  
  CHECK(ot1.contains(&(*pt2)));
  CHECK(ot1.contains(&(*pt3)));
  CHECK(ot1.contains(&(*pt4)));
  CHECK_FALSE(ot1.contains(&(*pt5)));
  CHECK_FALSE(ot1.contains(&(*pt6)));
}

TEST_CASE("OcTree random insertion and check","[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,3.00f,3.00f,3.00f);

  std::vector<std::shared_ptr<glibby::Point3>> valid;
  std::vector<std::shared_ptr<glibby::Point3>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-1.5f, 1.5f);

  for (int i=0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point3> temp(new glibby::Point3);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    temp->coord[2] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i=0; i < valid.size(); i++) 
  {
    CHECK(ot1.insert(&(*(valid[i]))));
    CHECK(ot1.contains(&(*(valid[i]))));
  }

  for (int i=0; i < 25; i++) 
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
  for (int i=0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(ot1.insert(&(*(invalid[i]))));
    CHECK_FALSE(ot1.contains(&(*(invalid[i]))));
  }
}

TEST_CASE("OcTree random insertion with larger capacity","[spatial][OcTree]") 
{
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,3.00f,3.00f,3.00f,5);

  std::vector<std::shared_ptr<glibby::Point3>> valid;
  std::vector<std::shared_ptr<glibby::Point3>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-1.5f, 1.5f);

  for (int i=0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point3> temp(new glibby::Point3);
    temp->coord[0] = distribution(gen);
    temp->coord[1] = distribution(gen);
    temp->coord[2] = distribution(gen);
    valid.push_back(temp);
  }
  for (int i=0; i < valid.size(); i++) 
  {
    CHECK(ot1.insert(&(*(valid[i]))));
    CHECK(ot1.contains(&(*(valid[i]))));
  }

  for (int i=0; i < 25; i++) 
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
  for (int i=0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(ot1.insert(&(*(invalid[i]))));
    CHECK_FALSE(ot1.contains(&(*(invalid[i]))));
  }
}

TEST_CASE("OcTree find all points in area","[spatial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,3.00f,3.00f,3.00f);

  std::vector<glibby::Point3> should_include;
  for (int i=-1; i <= 1; i++) 
  {
    for (int j=-1; j <= 1; j++) 
    {
      for (int k=-1; k <= 1; k++) 
      {
        glibby::Point3 temp;
        temp.coord[0] = i;
        temp.coord[1] = j;
        temp.coord[2] = k;
        should_include.push_back(temp);
        CHECK(ot1.insert(&temp));
        CHECK(ot1.contains(&temp));
      }
    }
  }

  std::vector<glibby::Point3> in_area = ot1.query(&(*pt1),3.00f,3.00f,3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i=0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j=0; j < in_area.size(); j++) 
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

TEST_CASE("OcTree find all points in randomized area","[spatial][OcTree]") {
  std::shared_ptr<glibby::Point3> pt1(new glibby::Point3);
  pt1->coord[0] = 0.00f;
  pt1->coord[1] = 0.00f;
  pt1->coord[2] = 0.00f;
  glibby::OcTree ot1 = glibby::OcTree(pt1,5.00f,5.00f,5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-2.5f, 2.5f);

  std::vector<glibby::Point3> all_points;
  for (int i=0; i < 1000; i++) 
  {
    glibby::Point3 temp;
    temp.coord[0] = distribution(gen);
    temp.coord[1] = distribution(gen);
    temp.coord[2] = distribution(gen);
    all_points.push_back(temp);
    CHECK(ot1.insert(&temp));
    CHECK(ot1.contains(&temp));
  }

  float width = 2*fabs(distribution(gen));
  float height = 2*fabs(distribution(gen));
  float depth = 2*fabs(distribution(gen));

  std::vector<glibby::Point3> in_area = ot1.query(&(*pt1),width,height,depth);
  std::vector<glibby::Point3> should_include;

  for (int i=0; i < all_points.size(); i++) 
  {
    if (all_points[i].coord[0] < width / 2 && 
        all_points[i].coord[0] > -1*width / 2 &&
        all_points[i].coord[1] < height / 2 && 
        all_points[i].coord[1] > -1*height / 2 &&
        all_points[i].coord[2] < depth / 2 && 
        all_points[i].coord[2] > -1*depth / 2)
    {
      should_include.push_back(all_points[i]);
    }
  }

  CHECK(in_area.size() == should_include.size());

  for (int i=0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j=0; j < in_area.size(); j++) 
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

