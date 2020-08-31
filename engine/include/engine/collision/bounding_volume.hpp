#pragma once

#include <math/vec3.hpp>
#include <limits>

const double LOWEST = std::numeric_limits<double>::lowest();
const double HIGHEST = std::numeric_limits<double>::max();

struct BoundingBox {
    Vec3 max = Vec3(LOWEST, LOWEST, LOWEST);
    Vec3 min = Vec3(HIGHEST, HIGHEST, HIGHEST);

    double Volume() const;

    int GetLargestDim() const;
};

BoundingBox Merge(const BoundingBox &lhs, const BoundingBox &rhs);

bool IsIntersect(const BoundingBox &lhs, const BoundingBox &rhs);

double IntersectionVolume(const BoundingBox &lhs, const BoundingBox &rhs);

double UnionVolume(const BoundingBox &lhs, const BoundingBox &rhs);

const Vec3 DOP_DIRS[] = {
        Vec3(1, 0, 0),
        Vec3(0, 1, 0),
        Vec3(0, 0, 1),
        Vec3(1, 1, 1),
        Vec3(1, 1, -1),
        Vec3(-1, 1, 1),
        Vec3(-1, 1, -1)
};
const int DOP_DIR_NUM = sizeof(DOP_DIRS)/sizeof(Vec3);

struct DOP {
    double min[DOP_DIR_NUM];
    double max[DOP_DIR_NUM];
};
