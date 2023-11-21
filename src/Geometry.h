#pragma once
#include "utils.h"
#include <vector>

class Geometry
{
public:
    std::vector<Vec3> vertexArray;
    std::vector<int> indices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texCoords;
};