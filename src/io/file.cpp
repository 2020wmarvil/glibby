//
// Created by Matthew McCall on 1/27/23.
//

#include "glibby/io/tri_mesh.h"
#include "glibby/primitives/triangle3D.h"
#include "glibby/io/file.h"

#include <fstream>

namespace glibby
{
    TriMesh LoadFromObjFile(const std::string &path) {
        // Load file at path
        std::ifstream file { path };

        std::vector<Point3D> vertices;
        std::vector<Triangle3D> triangles;

        std::string keyword;
        while (file >> keyword)
        {
            if (keyword == "#")
            {
                // Comment
                std::string comment;
                std::getline(file, comment);
            }
            else if (keyword == "v")
            {
                // Vertex
                float x, y, z;
                file >> x >> y >> z;

                vertices.push_back({x, y, z});
            }
            else if (keyword == "vt")
            {
                // Texture coordinate
                float u, v;
                file >> u >> v;
            }
            else if (keyword == "vn")
            {
                // Normal
                float x, y, z;
                file >> x >> y >> z;
            }
            else if (keyword == "f")
            {
                // Face
                int a, b, c;
                file >> a >> b >> c;
                triangles.push_back({vertices[a], vertices[b], vertices[c]});
            }
        }

        return { triangles };
    }
}