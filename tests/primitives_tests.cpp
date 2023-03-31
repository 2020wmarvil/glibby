#include "glibby/math/general_math.h"
#include "glibby/primitives/point.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// To execute all tests: ./glibby_test_suite --order rand --warn NoAssertions
// Useful options:
// <test_name>
// [<tag_name>]
// --skip-benchmarks
// --break (enters the debugger when a test fails)
// e.g. ./glibby_test_suite [primitive][point2D] --skip-benchmarks

TEST_CASE("Point1D distance", "[primitive][point]") {
    glibby::Point1 p1;
    glibby::Point1 p2({ 5.0f });

    CHECK(glibby::EuclideanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::EuclideanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::EuclideanDistance(p1, p2), Catch::Matchers::WithinAbs(5.0f, FLT_NEAR_ZERO));

    CHECK(glibby::ManhattanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::ManhattanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::ManhattanDistance(p1, p2), Catch::Matchers::WithinAbs(5.0f, FLT_NEAR_ZERO));
}

TEST_CASE("Point2D distance", "[primitive][point]") {
    glibby::Point2 p1;
    glibby::Point2 p2({5.0f, 15.0f});

    CHECK(glibby::EuclideanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::EuclideanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::EuclideanDistance(p1, p2), Catch::Matchers::WithinAbs(15.81139f, FLT_NEAR_ZERO));

    CHECK(glibby::ManhattanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::ManhattanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::ManhattanDistance(p1, p2), Catch::Matchers::WithinAbs(20.0f, FLT_NEAR_ZERO));
}

TEST_CASE("Point3D distance", "[primitive][point]") {
    glibby::Point3 p1;
    glibby::Point3 p2({ 5.0f, 15.0f, 10.0f });

    CHECK(glibby::EuclideanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::EuclideanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::EuclideanDistance(p1, p2), Catch::Matchers::WithinAbs(18.7082869339f, FLT_NEAR_ZERO));

    CHECK(glibby::ManhattanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::ManhattanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::ManhattanDistance(p1, p2), Catch::Matchers::WithinAbs(30.0f, FLT_NEAR_ZERO));
}

TEST_CASE("PointND distance", "[primitive][point]") {
    glibby::Point<float, 5> p1;
    glibby::Point<float, 5> p2({5.0f, 15.0f, 3.0f, 7.0f, 6.0f});

    CHECK(glibby::EuclideanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::EuclideanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::EuclideanDistance(p1, p2), Catch::Matchers::WithinAbs(18.547236991f, FLT_NEAR_ZERO));

    CHECK(glibby::ManhattanDistance(p1, p1) == 0);
    CHECK_FALSE(glibby::ManhattanDistance(p1, p2) == 0);
    CHECK_THAT(glibby::ManhattanDistance(p1, p2), Catch::Matchers::WithinAbs(36.0f, FLT_NEAR_ZERO));
}

/*TEST_CASE("Point2D distance", "[!benchmark][primitive][point2D]") {
    BENCHMARK("Point2D distance") {
        return glibby::Distance({ 0.0f, 0.0f }, { 5.0f, 8.0f });
    };
<<<<<<< HEAD
}*/
