#include "../include/engine/model/objects/box.hpp"

Box::Box(double w, double h, double d)
    : Polyhedron(GetBoxVertices(w, h, d), GetBoxFaces()), width(w), height(h), depth(d) {}

void Box::RecalculateInertia() {
    if (fixed) {
        inertia = ObjectInertia();
    } else {
        inertia.mass = width * height * depth * physics_material.density;
        inertia.mass_inv = 1.0 / inertia.mass;

        inertia.moment_of_inertia = Mat3();
        inertia.moment_of_inertia[0][0] = inertia.mass * (height * height + depth * depth) / 12;
        inertia.moment_of_inertia[1][1] = inertia.mass * (width * width + depth * depth) / 12;
        inertia.moment_of_inertia[2][2] = inertia.mass * (height * height + width * width) / 12;
        inertia.moment_of_inertia_inv = inertia.moment_of_inertia.Inverse();
    }
}

std::vector<Face> GetBoxFaces() {
    return {
        Face{7, 6, 2},
        Face{2, 3, 7},
        Face{0, 4, 5},
        Face{5, 1, 0},
        Face{5, 4, 6},
        Face{6, 7, 5},
        Face{2, 0, 1},
        Face{1, 3, 2},
        Face{7, 3, 1},
        Face{1, 5, 7},
        Face{0, 2, 6},
        Face{6, 4, 0},
    };
}

std::vector<Vec3> GetBoxVertices(double w, double h, double d) {
    return {
        Vec3(-w * 0.5, -h * 0.5, -d * 0.5),
        Vec3(-w * 0.5, -h * 0.5, +d * 0.5),
        Vec3(-w * 0.5, +h * 0.5, -d * 0.5),
        Vec3(-w * 0.5, +h * 0.5, +d * 0.5),
        Vec3(+w * 0.5, -h * 0.5, -d * 0.5),
        Vec3(+w * 0.5, -h * 0.5, +d * 0.5),
        Vec3(+w * 0.5, +h * 0.5, -d * 0.5),
        Vec3(+w * 0.5, +h * 0.5, +d * 0.5),
    };
}

std::vector<Vec3> GetBoxVertices(const BoundingBox& bbox) {
    double w = bbox.max.x - bbox.min.x;
    double h = bbox.max.y - bbox.min.y;
    double d = bbox.max.z - bbox.min.z;
    auto vertices = GetBoxVertices(w, h, d);
    for (auto& v : vertices) {
        v += bbox.min + Vec3(w, h, d) * 0.5;
    }
    return vertices;
}
