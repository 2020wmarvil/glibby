#include "glibby/spatial/kdTree.h"
#include "glibby/primitives/point.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("kdTree Printed", "[spaial][kdTree]") {
        glibby::Point3 p1;
        glibby::Point3 p2({ 5.0f, 15.0f, 10.0f });
        glibby::Point3 p3({ 11.0f, 16.0f, 8.0f});
        glibby::Point3 p4({ 6.0f, 8.0f, 9.0f });
        glibby::Point3 p5({ 21.0f, 19.0f, 78.0f });
        glibby::kdTree<float, 3> KT({p1, p2, p3, p4, p5});
        glibby::Point3 p6({ 21.0f, 18.0f, 78.0f });
        KT.BFSprint();
        KT.insert(p6);
        KT.BFSprint();
}