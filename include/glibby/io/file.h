//
// Created by Matthew McCall on 1/27/23.
//

#ifndef GLIBBY_FILE_H
#define GLIBBY_FILE_H

#include <string>

namespace glibby
{
    class TriMesh;

    TriMesh LoadFromObjFile(const std::string& path);
}

#endif //GLIBBY_FILE_H
