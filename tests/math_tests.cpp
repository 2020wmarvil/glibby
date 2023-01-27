#include "glibby/math/general_math.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Vec2 Operations", "[math][vec2]") {
    const glibby::Vec2 zeroVec;
    const glibby::Vec2 oneVec(1.0f);

    CHECK(zeroVec.x == 0.0f && zeroVec.y == 0.0f);
    CHECK(oneVec.x == 1.0f && oneVec.y == 1.0f);

    const float x1 = 1.0f;
    const float y1 = 2.0f;
    const float x2 = 10.0f;
    const float y2 = 6.0f;

    const glibby::Vec2 v1(x1, y1);
    const glibby::Vec2 v2(x2, y2);
    const glibby::Vec2 v1v2Addition(x1 + x2, y1 + y2);
    const glibby::Vec2 v1v2Subtraction(x1 - x2, y1 - y2);
    const glibby::Vec2 v2v1Subtraction(x2 - x1, y2 - y1);

    const float v1SquareMagnitude = std::sqrt(x1 * x1 + y1 * y1);
    const float v1Magnitude = std::sqrt(x1 * x1 + y1 * y1);

    float v1v2DotProduct = x1 * x2 + y1 * y2;

    // negate
    // +=
    // +
    // *=
    // *
    // /
    // /=


    //CHECK(glibby::Distance(p1, p1) == 0);
    //CHECK_FALSE(glibby::Distance(p1, p2) == 0);
    //CHECK_THAT(glibby::Distance(p1, p2), Catch::Matchers::WithinAbs(15.81139f, FLT_NEAR_ZERO));
}

TEST_CASE("Vec3 Operations", "[math][vec3]") {
}

TEST_CASE("Vec3 Operations", "[math][vec4]") {
}
