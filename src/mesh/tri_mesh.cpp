//
// Created by Matthew McCall on 1/27/23.
//

#include "glibby/primitives/triangle3D.h"
#include "glibby/mesh/tri_mesh.h"

namespace glibby
{
    TriMesh::TriMesh(const std::vector<Triangle3D> &vertices) : triangles(vertices)
    {
    }

    const std::vector<Triangle3D> &TriMesh::getTriangles() const {
        return triangles;
    }
}