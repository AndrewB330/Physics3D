#include <engine/utils.hpp>

std::vector<Triple> GetBoxFaces() {
    return {
            {7, 6, 2},
            {2, 3, 7},
            {0, 4, 5},
            {5, 1, 0},
            {5, 4, 6},
            {6, 7, 5},
            {2, 0, 1},
            {1, 3, 2},
            {7, 3, 1},
            {1, 5, 7},
            {0, 2, 6},
            {6, 4, 0},
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

std::vector<Vec3> GetBoxVertices(const BoundingBox &bbox) {
    double w = bbox.max.x - bbox.min.x;
    double h = bbox.max.y - bbox.min.y;
    double d = bbox.max.z - bbox.min.z;
    auto vertices = GetBoxVertices(w, h, d);
    for (auto &v : vertices) {
        v += bbox.min + Vec3(w, h, d) * 0.5;
    }
    return vertices;
}
