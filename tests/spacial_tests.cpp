#include "glibby/primitives/point2D.h"
#include "glibby/spacial/QuadTree.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>
#include <vector>
#include <random>

TEST_CASE("QuadTree insert point","[spacial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.49f;
  pt4->y = -1.49f;
  CHECK(qt1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)));
}

TEST_CASE("QuadTree remove point","[spacial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(&(*pt2)));
  CHECK(qt1.remove(&(*pt2)));

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(&(*pt3)));
  CHECK(qt1.remove(&(*pt3)));
  
  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.49f;
  pt4->y = -1.49f;
  CHECK(qt1.insert(&(*pt4)));
  CHECK(qt1.remove(&(*pt4)));
  
  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)));
  CHECK_FALSE(qt1.remove(&(*pt5)));

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)));
  CHECK_FALSE(qt1.remove(&(*pt6)));
}


TEST_CASE("QuadTree contains point","[spacial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f); 

  std::shared_ptr<glibby::Point2D> pt2(new glibby::Point2D);
  pt2->x = 1.00f;
  pt2->y = 1.00f;
  CHECK(qt1.insert(&(*pt2)));

  std::shared_ptr<glibby::Point2D> pt3(new glibby::Point2D);
  pt3->x = -1.00f;
  pt3->y = -1.00f;
  CHECK(qt1.insert(&(*pt3)));
  
  std::shared_ptr<glibby::Point2D> pt4(new glibby::Point2D);
  pt4->x = 1.45f;
  pt4->y = -1.45f;
  CHECK(qt1.insert(&(*pt4)));
  
  std::shared_ptr<glibby::Point2D> pt5(new glibby::Point2D);
  pt5->x = 3.00f;
  pt5->y = -3.00f;
  CHECK_FALSE(qt1.insert(&(*pt5)));

  std::shared_ptr<glibby::Point2D> pt6(new glibby::Point2D);
  pt6->x = -10.00f;
  pt6->y = -10.00f;
  CHECK_FALSE(qt1.insert(&(*pt6)));

  CHECK(qt1.contains(&(*pt2)));
  CHECK(qt1.contains(&(*pt3)));
  CHECK(qt1.contains(&(*pt4)));
  CHECK_FALSE(qt1.contains(&(*pt5)));
  CHECK_FALSE(qt1.contains(&(*pt6)));
}

TEST_CASE("QuadTree random insertion and check","[spacial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f);

  std::vector<std::shared_ptr<glibby::Point2D>> valid;
  std::vector<std::shared_ptr<glibby::Point2D>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-1.5f, 1.5f);

  for (int i=0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point2D> temp(new glibby::Point2D);
    temp->x = distribution(gen);
    temp->y = distribution(gen);
    valid.push_back(temp);
  }
  for (int i=0; i < valid.size(); i++) 
  {
    CHECK(qt1.insert(&(*(valid[i]))));
    CHECK(qt1.contains(&(*(valid[i]))));
  }

  for (int i=0; i < 25; i++) 
  {
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
  for (int i=0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))));
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))));
  }

}

TEST_CASE("QuadTree larger capacity per node","[spacial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f,5);

  std::vector<std::shared_ptr<glibby::Point2D>> valid;
  std::vector<std::shared_ptr<glibby::Point2D>> invalid;

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-1.5f, 1.5f);

  for (int i=0; i < 100; i++) 
  {
    std::shared_ptr<glibby::Point2D> temp(new glibby::Point2D);
    temp->x = distribution(gen);
    temp->y = distribution(gen);
    valid.push_back(temp);
  }
  for (int i=0; i < valid.size(); i++) 
  {
    CHECK(qt1.insert(&(*(valid[i]))));
    CHECK(qt1.contains(&(*(valid[i]))));
  }

  for (int i=0; i < 25; i++) 
  {
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
  for (int i=0; i < invalid.size(); i++) 
  {
    CHECK_FALSE(qt1.insert(&(*(invalid[i]))));
    CHECK_FALSE(qt1.contains(&(*(invalid[i]))));
  }

}


TEST_CASE("QuadTree find all points in area","[spacial][QuadTree]") {
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,3.00f,3.00f);

  std::vector<glibby::Point2D> should_include;
  for (int i=-1; i <= 1; i++) 
  {
    for (int j=-1; j <= 1; j++) 
    {
      glibby::Point2D temp;
      temp.x = i;
      temp.y = j;
      should_include.push_back(temp);
      CHECK(qt1.insert(&temp));
      CHECK(qt1.contains(&temp));
    }
  }

  std::vector<glibby::Point2D> in_area = qt1.query(&(*pt1),3.00f,3.00f);

  CHECK(in_area.size() == should_include.size());

  for (int i=0; i < should_include.size(); i++) 
  {
    bool in = false;
    for (int j=0; j < in_area.size(); j++) 
    {
      if (should_include[i].x == in_area[j].x &&
          should_include[i].y == in_area[j].y) 
      {
        in = true;
        continue;
      }
    }
    CHECK(in);
  }
}

TEST_CASE("QuadTree find all randomized points in area","[spacial][QuadTree]") 
{
  std::shared_ptr<glibby::Point2D> pt1(new glibby::Point2D);
  pt1->x = 0.00f;
  pt1->y - 0.00f;
  glibby::QuadTree qt1 = glibby::QuadTree(pt1,5.00f,5.00f);

  std::default_random_engine gen;
  std::uniform_real_distribution<float>  distribution(-2.5f, 2.5f);

  std::vector<glibby::Point2D> all_points;
  for (int i=0; i < 100; i++) 
  {
    glibby::Point2D temp;
    temp.x = distribution(gen);
    temp.y = distribution(gen);
    all_points.push_back(temp);
    CHECK(qt1.insert(&temp));
    CHECK(qt1.contains(&temp));
  }

  float width = distribution(gen);
  float height = distribution(gen);

  std::vector<glibby::Point2D> in_area = qt1.query(&(*pt1),width,height);
  std::vector<glibby::Point2D> should_include;

  for (int i=0; i < all_points.size(); i++) 
  {
    if (all_points[i].x < width / 2 && all_points[i].x > -1*width / 2 &&
        all_points[i].y < height / 2 && all_points[i].y > -1*height / 2) 
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
      if (should_include[i].x == in_area[j].x &&
          should_include[i].y == in_area[j].y) 
      {
        in = true;
        continue;
      }
    }
    CHECK(in);
  }
}
