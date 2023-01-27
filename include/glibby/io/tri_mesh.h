//
// Created by Matthew McCall on 1/27/23.
//

#ifndef GLIBBY_TRI_MESH_H
#define GLIBBY_TRI_MESH_H

#include <vector>

namespace glibby {

    struct Point3D;

    class TriMesh
    {
    public:
        TriMesh(const std::vector<Point3D>& vertices);

    private:
        std::vector<Point3D> vertices;
    };
}


#endif //GLIBBY_TRI_MESH_H
