//
// Created by Matthew McCall on 1/27/23.
//

#ifndef GLIBBY_TRI_MESH_H
#define GLIBBY_TRI_MESH_H

#include <vector>

namespace glibby {

    struct Triangle3D;

    class TriMesh
    {
    public:
        TriMesh(const std::vector<Triangle3D>& vertices);

        const std::vector<Triangle3D> &getTriangles() const;

    private:
        std::vector<Triangle3D> triangles;
    };
}


#endif //GLIBBY_TRI_MESH_H
