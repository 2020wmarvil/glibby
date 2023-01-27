//
// Created by Matthew McCall on 1/27/23.
//

#include "glibby/primitives/point3D.h"
#include "glibby/io/tri_mesh.h"

namespace glibby
{
    TriMesh::TriMesh(const std::vector<Point3D> &vertices) : vertices(vertices)
    {
    }
}