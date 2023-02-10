#include "glibby/math/complex_math.h"
#include "glibby/math/general_math.h"
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

TEST_CASE("ComplexNumber operator+", "[math][complex_math]") {
    glibby::ComplexNumber n1 = { 1.0f, 2.0f };
    glibby::ComplexNumber n2 = { 3.0f, 4.0f };
    glibby::ComplexNumber n3 = { 4.0f, 6.0f };

    CHECK((n1 + n2) == n3);
}

TEST_CASE("ComplexNumber operator-", "[math][complex_math]") {
    glibby::ComplexNumber n1 = { 1.0f, 2.0f };
    glibby::ComplexNumber n2 = { 3.0f, 4.0f };
    glibby::ComplexNumber n3 = { -2.0f, -2.0f }; 


    CHECK(n1 - n2 == n3);
}

TEST_CASE("ComplexNumber operator*", "[math][complex_math]") {
    glibby::ComplexNumber n1 = { 1.0f, 2.0f };
    glibby::ComplexNumber n2 = { 5.0f, 15.0f };
    glibby::ComplexNumber n3 = { -25.0f, 25.0f };

    CHECK(n1 * n2 == n3);
}
TEST_CASE("ComplexNumber operator/", "[math][complex_math]") {
    glibby::ComplexNumber n1 = { 1.0f, 2.0f };
    glibby::ComplexNumber n2 = { 3.0f, 4.0f };
    glibby::ComplexNumber n3 = n1 / n2;
    glibby::ComplexNumber result = { 0.44f, 0.08f };

    CHECK(n3 == result);
    CHECK_THAT(n3.a, Catch::Matchers::WithinAbs(result.a, FLT_NEAR_ZERO));
    CHECK_THAT(n3.b, Catch::Matchers::WithinAbs(result.b, FLT_NEAR_ZERO));
}
TEST_CASE("ComplexNumber Conjugate", "[math][complex_math]") {
    glibby::ComplexNumber n1 = { 7.0f, 5.0f };
    glibby::ComplexNumber n2 = { 7.0f, -5.0f };
    CHECK(conjugate(n1) == n2);
}

TEST_CASE("ComplexNumber Exponential", "[math][complex_math]") {
    glibby::ComplexNumber n1 = { 1.0f, 2.0f };
    glibby::ComplexNumber n2 = { 3.0f, 4.0f };
    glibby::ComplexNumber nr = complex_pow(n1, n2);
    glibby::ComplexNumber n3 = { 0.129009f, 0.033924f };

    CHECK_THAT(nr.a, Catch::Matchers::WithinAbs(n3.a, FLT_NEAR_ZERO));
    CHECK_THAT(nr.b, Catch::Matchers::WithinAbs(n3.b, FLT_NEAR_ZERO));
}

TEST_CASE("ComplexNumber Argument", "[math][complex_math]") {
    glibby::ComplexNumber n = { 5.0f, 7.0f };
    float result = 0.950546f;

    CHECK_THAT(glibby::arg(n), Catch::Matchers::WithinAbs(result, FLT_NEAR_ZERO));
}

TEST_CASE("ComplexNumber Absolute", "[math][complex_math]") {
    glibby::ComplexNumber n = { 3.0f, 4.0f };
    float result = 5.0f;

    CHECK_THAT(glibby::absolute(n), Catch::Matchers::WithinAbs(result, FLT_NEAR_ZERO));
}