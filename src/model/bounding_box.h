#pragma once
#include "../math/vec3.h"
#include <limits>

const double LOWEST = std::numeric_limits<double>::lowest();
const double HIGHEST = std::numeric_limits<double>::max();

struct BoundingBox {
    Vec3 max = Vec3(LOWEST, LOWEST, LOWEST);
    Vec3 min = Vec3(HIGHEST, HIGHEST, HIGHEST);

    double Volume() const;
};

BoundingBox Merge(const BoundingBox& lhs, const BoundingBox& rhs);

bool IsIntersect(const BoundingBox& lhs, const BoundingBox& rhs);

double IntersectionVolume(const BoundingBox& lhs, const BoundingBox& rhs);

double UnionVolume(const BoundingBox& lhs, const BoundingBox& rhs);