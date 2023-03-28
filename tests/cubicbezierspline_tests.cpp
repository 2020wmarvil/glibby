#include "glibby/splines/CubicBezierSpline.h"
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

TEST_CASE("Cubic Bezier Curve Point Assignment", "[!benchmark][splines][CubicBezierSpline]") 
{
    glibby::Point<float, 2> bezierPoints[4];
    
    bezierPoints[0] = glibby::Point<float, 2>({ 0.0f, 0.0f });
    bezierPoints[1] = glibby::Point<float, 2>({ 1.0f, 2.0f });
    bezierPoints[2] = glibby::Point<float, 2>({ 3.0f, 2.0f });
    bezierPoints[3] = glibby::Point<float, 2>({ 4.0f, 0.0f });

    glibby::CubicBezierSpline<float, 2> spline(bezierPoints);

    // Waiting on point equivalency to be implemented.
    CHECK(spline.getPoint(0) == bezierPoints[0]);
    CHECK(spline.getPoint(1) == bezierPoints[1]);
    CHECK(spline.getPoint(2) == bezierPoints[2]);
    CHECK(spline.getPoint(3) == bezierPoints[3]);
}
