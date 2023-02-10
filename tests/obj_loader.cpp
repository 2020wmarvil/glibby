#include <glibby/io/file.h>
#include <glibby/mesh/tri_mesh.h>
#include <glibby/primitives/triangle3D.h>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Load Cube from OBJ", "[loaders][obj]") {
    glibby::TriMesh mesh = glibby::LoadFromObjFile("resources/cube.obj");
    CHECK(mesh.getTriangles().size() == 12);
}