#include "glibby/math/polynomial.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <vector>

TEST_CASE("Polynomial operator-", "[math][polynomial]"){
    vector<int> values1 {3,4,9,1};
    vector<int> values2 {5,1,2};
    vector<int> values3 {-2,3,7,1};
    
    glibby::Quadratic n1(values1);
    glibby::Quadratic n2(values2);
    glibby::Quadratic n3(values3);

    CHECK((n1 - n2) == n3);
}

TEST_CASE("Polynomial operator+", "[math][polynomial]"){
    vector<int> values1 {3,4,9,1};
    vector<int> values2 {5,1,2};
    vector<int> values3 {8,5,11,1};
    
    glibby::Quadratic n1(values1);
    glibby::Quadratic n2(values2);
    glibby::Quadratic n3(values3);

    CHECK((n1 + n2) == n3);
}

TEST_CASE("Polynomial operator*", "[math][polynomial]"){
    vector<int> values1 {3,4,9,1};
    vector<int> values2 {5,1,2};
    vector<int> values3 {15,23,55,22,19,2};
    
    glibby::Quadratic n1(values1);
    glibby::Quadratic n2(values2);
    glibby::Quadratic n3(values3);

    CHECK((n1 * n2) == n3);
}

TEST_CASE("Polynomial evaluate", "[math][polynomial]"){
    vector<int> values1 {3,4,9,1};
    vector<int> values2 {5,1,2};
    vector<int> values3 {-1,2,-6,2};
    vector<int> values4 {1,3,2};
    vector<int> values5 {2,9,4};

    glibby::Quadratic n1(values1);
    glibby::Quadratic n2(values2);
    glibby::Quadratic n3(values3);
    glibby::Quadratic n4(values4);
    glibby::Quadratic n5(values5);

    CHECK(n1.evaluate(0) == 3);
    CHECK(n1.horner(2) == 55);
    CHECK(n2.evaluate(4) == 41);
    CHECK(n2.horner(3) == 26);
    CHECK(n3.evaluate(3) == 5);
    CHECK(n3.horner(1) == -3);
    CHECK(n4.evaluate(2) == 15);
    CHECK(n5.evaluate(-9) == 245);
}

TEST_CASE("Polynomial root finding", "[math][polynomial]"){
    vector<int> values6 {6,5,1};
    vector<int> values7 {-3,-5,2};

    glibby::Quadratic n6(values6);
    glibby::Quadratic n7(values7);

    vector<double> six_roots = n6.findRoots();
    CHECK(six_roots[0] == -2);
    CHECK(six_roots[1] == -3);

    vector<double> seven_roots = n7.findRoots();
    CHECK(seven_roots[0] == 3);
    CHECK(seven_roots[1] == -0.5);
}