#include "glibby/math/general_math.h"
#include "glibby/primitives/point2D.h"
#include "glibby/primitives/circle2D.h"

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

TEST_CASE("Point2D distance", "[primitive][point2D]") {
    glibby::Point2D p1 = { 0.0f, 0.0f };
    glibby::Point2D p2 = { 5.0f, 15.0f };

    CHECK(glibby::Distance(p1, p1) == 0);
    CHECK_FALSE(glibby::Distance(p1, p2) == 0);
    CHECK_THAT(glibby::Distance(p1, p2), Catch::Matchers::WithinAbs(15.81139f, FLT_NEAR_ZERO));
}

TEST_CASE("Point2D distance", "[!benchmark][primitive][point2D]") {
    BENCHMARK("Point2D distance") {
        glibby::Point2D p1 = { 0.0f, 0.0f };
        glibby::Point2D p2 = { 5.0f, 15.0f };
        return glibby::Distance(p1, p2);
    };
}