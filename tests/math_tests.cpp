#include "glibby/math/general_math.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace glibby;

TEST_CASE("Vec2 Operations", "[math][vec2]")
{
    const Vec2 zeroVec;
    const Vec2 oneVec(1.0f);
    const Vec2 simpleVec(1.0f, 2.0f);
    const Vec2 negativeSimpleVec = -simpleVec;

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(simpleVec - negativeSimpleVec == simpleVec * 2);
    CHECK(simpleVec - negativeSimpleVec == 2 * simpleVec);
    CHECK(simpleVec / 5.0f == Vec2(1.0f / 5.0f, 2.0f / 5.0f));

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(Dot(simpleVec, negativeSimpleVec) == -5.0f);
    CHECK(SquareMagnitude(simpleVec) == 5.0f);
    CHECK_THAT(Magnitude(simpleVec), Catch::Matchers::WithinAbs(sqrt(5.0f), FLT_NEAR_ZERO));
    CHECK_THAT(Magnitude(Normalize(simpleVec)), Catch::Matchers::WithinAbs(1.0f, FLT_NEAR_ZERO));

    Vec2 copyConstructedVec(simpleVec);
    Vec2 copyAssignmentVec = simpleVec;

    copyConstructedVec += copyAssignmentVec;
    CHECK(copyConstructedVec == 2.0f * simpleVec);

    copyConstructedVec -= copyAssignmentVec;
    CHECK(copyConstructedVec == simpleVec);

    copyConstructedVec *= 5.0f;
    CHECK(copyConstructedVec == 5.0f * simpleVec);

    copyConstructedVec /= 5.0f;
    CHECK(copyConstructedVec == simpleVec);
}

TEST_CASE("Vec3 Operations", "[math][vec3]")
{
    const Vec3 zeroVec;
    const Vec3 oneVec(1.0f);
    const Vec3 simpleVec(1.0f, 2.0f, 3.0f);
    const Vec3 negativeSimpleVec = -simpleVec;

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(simpleVec - negativeSimpleVec == simpleVec * 2);
    CHECK(simpleVec - negativeSimpleVec == 2 * simpleVec);
    CHECK(simpleVec / 5.0f == Vec3(1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f));

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(Dot(simpleVec, negativeSimpleVec) == -14.0f);
    CHECK(SquareMagnitude(simpleVec) == 14.0f);
    CHECK_THAT(Magnitude(simpleVec), Catch::Matchers::WithinAbs(sqrt(14.0f), FLT_NEAR_ZERO));
    CHECK_THAT(Magnitude(Normalize(simpleVec)), Catch::Matchers::WithinAbs(1.0f, FLT_NEAR_ZERO));
    CHECK(Cross(simpleVec, negativeSimpleVec) == zeroVec);
    CHECK(Cross(simpleVec, oneVec) == Vec3(-1.0f, 2.0f, -1.0f));
    CHECK(Cross(oneVec, simpleVec) == Vec3(1.0f, -2.0f, 1.0f));

    Vec3 copyConstructedVec(simpleVec);
    Vec3 copyAssignmentVec = simpleVec;

    copyConstructedVec += copyAssignmentVec;
    CHECK(copyConstructedVec == 2.0f * simpleVec);

    copyConstructedVec -= copyAssignmentVec;
    CHECK(copyConstructedVec == simpleVec);

    copyConstructedVec *= 5.0f;
    CHECK(copyConstructedVec == 5.0f * simpleVec);

    copyConstructedVec /= 5.0f;
    CHECK(copyConstructedVec == simpleVec);
}

TEST_CASE("Vec4 Operations", "[math][vec4]")
{
    const Vec4 zeroVec;
    const Vec4 oneVec(1.0f);
    const Vec4 simpleVec(1.0f, 2.0f, 3.0f, 4.0f);
    const Vec4 negativeSimpleVec = -simpleVec;

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(simpleVec - negativeSimpleVec == simpleVec * 2);
    CHECK(simpleVec - negativeSimpleVec == 2 * simpleVec);
    CHECK(simpleVec / 5.0f == Vec4(1.0f / 5.0f, 2.0f / 5.0f, 3.0f / 5.0f, 4.0f / 5.0f));

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(Dot(simpleVec, negativeSimpleVec) == -30.0f);
    CHECK(SquareMagnitude(simpleVec) == 30.0f);
    CHECK_THAT(Magnitude(simpleVec), Catch::Matchers::WithinAbs(sqrt(30.0f), FLT_NEAR_ZERO));
    CHECK_THAT(Magnitude(Normalize(simpleVec)), Catch::Matchers::WithinAbs(1.0f, FLT_NEAR_ZERO));

    Vec4 copyConstructedVec(simpleVec);
    Vec4 copyAssignmentVec = simpleVec;

    copyConstructedVec += copyAssignmentVec;
    CHECK(copyConstructedVec == 2.0f * simpleVec);

    copyConstructedVec -= copyAssignmentVec;
    CHECK(copyConstructedVec == simpleVec);

    copyConstructedVec *= 5.0f;
    CHECK(copyConstructedVec == 5.0f * simpleVec);

    copyConstructedVec /= 5.0f;
    CHECK(copyConstructedVec == simpleVec);
}

TEST_CASE("VecN Operations", "[math][vecN]")
{
    const Vec<float, 10> zeroVec;
    const Vec<float, 10> oneVec(1.0f);
    float simpleArr[10] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };
    const Vec<float, 10> simpleVec(simpleArr);
    const Vec<float, 10> negativeSimpleVec = -simpleVec;

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(simpleVec - negativeSimpleVec == simpleVec * 2);
    CHECK(simpleVec - negativeSimpleVec == 2 * simpleVec);

    for (size_t i = 0; i < 10; i++)
    {
        simpleArr[i] /= 5.0f;
    }
    CHECK(simpleVec / 5.0f == Vec<float, 10>(simpleArr));

    CHECK(simpleVec + negativeSimpleVec == zeroVec);
    CHECK(Dot(simpleVec, negativeSimpleVec) == -385.0f);
    CHECK(SquareMagnitude(simpleVec) == 385.0f);
    CHECK_THAT(Magnitude(simpleVec), Catch::Matchers::WithinAbs(sqrt(385.0f), FLT_NEAR_ZERO));
    CHECK_THAT(Magnitude(Normalize(simpleVec)), Catch::Matchers::WithinAbs(1.0f, FLT_NEAR_ZERO));

    Vec<float, 10> copyConstructedVec(simpleVec);
    Vec<float, 10> copyAssignmentVec = simpleVec;

    copyConstructedVec += copyAssignmentVec;
    CHECK(copyConstructedVec == 2.0f * simpleVec);

    copyConstructedVec -= copyAssignmentVec;
    CHECK(copyConstructedVec == simpleVec);

    copyConstructedVec *= 5.0f;
    CHECK(copyConstructedVec == 5.0f * simpleVec);

    copyConstructedVec /= 5.0f;
    CHECK(copyConstructedVec == simpleVec);
}
