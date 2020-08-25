#include "polyhedron.h"

Polyhedron::Polyhedron(std::vector<Vec3> vertices, std::vector<Face> faces) :Object(), vertices(vertices), faces(faces) {
    for (auto & f : this->faces) {
        f.normal = Cross(vertices[f.v1] - vertices[f.v0], vertices[f.v2] - vertices[f.v0]).Norm();
    }
}

Face::Face(int v0, int v1, int v2):v0(v0), v1(v1), v2(v2) {}
