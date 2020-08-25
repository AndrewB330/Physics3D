#pragma once
#include <vector>
#include "object.h"

struct Face {
    int v0, v1, v2;
    Vec3 normal;

    Face(int v0 = 0, int v1 = 0, int v2 = 0);
};

class Polyhedron : public Object {
public:
    virtual ~Polyhedron() = default;

    Polyhedron(std::vector<Vec3> vertices, std::vector<Face> faces);
    std::vector<Vec3> vertices;
    std::vector<Face> faces;
};