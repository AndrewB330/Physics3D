#include <algorithm>
#include <engine/collision/bounding_volume.hpp>

double BoundingBox::Volume() const {
    double w = max.x - min.x;
    double h = max.y - min.y;
    double d = max.z - min.z;
    return (w >= 0 && h >= 0 && d >= 0 ? w * h * d : 0.0);
}

int BoundingBox::GetLargestDim() const {
    Vec3 size = max - min;
    return (size.x > size.y && size.x > size.z ? 0 : (size.y > size.z ? 1 : 2));
}

BoundingBox Merge(const BoundingBox &lhs, const BoundingBox &rhs) {
    BoundingBox box;
    box.min.x = std::min(lhs.min.x, rhs.min.x);
    box.min.y = std::min(lhs.min.y, rhs.min.y);
    box.min.z = std::min(lhs.min.z, rhs.min.z);
    box.max.x = std::max(lhs.max.x, rhs.max.x);
    box.max.y = std::max(lhs.max.y, rhs.max.y);
    box.max.z = std::max(lhs.max.z, rhs.max.z);
    return box;
}

bool IsIntersect(double l1, double r1, double l2, double r2) {
    return std::max(l2, l1) <= std::min(r1, r2);
}

bool IsIntersect(const BoundingBox &lhs, const BoundingBox &rhs) {
    return IsIntersect(lhs.min.x, lhs.max.x, rhs.min.x, rhs.max.x) &&
           IsIntersect(lhs.min.y, lhs.max.y, rhs.min.y, rhs.max.y) &&
           IsIntersect(lhs.min.z, lhs.max.z, rhs.min.z, rhs.max.z);
}

double IntersectionVolume(const BoundingBox &lhs, const BoundingBox &rhs) {
    if (!IsIntersect(lhs, rhs)) {
        return 0.0;
    }
    double w = std::min(lhs.max.x, rhs.max.x) - std::max(lhs.min.x, rhs.min.x);
    double h = std::min(lhs.max.y, rhs.max.y) - std::max(lhs.min.y, rhs.min.y);
    double d = std::min(lhs.max.z, rhs.max.z) - std::max(lhs.min.z, rhs.min.z);
    return w * h * d;
}
