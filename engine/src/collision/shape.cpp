#include <engine/collision/shape.hpp>

Vec3 ConvexShape::GetSupportingVector(const Vec3 &dir) const {
    return Vec3();
}

Vec3 SphereShape::GetSupportingVector(const Vec3 &dir) const {
    return dir.Norm() * radius;
}

SphereShape::SphereShape(double radius) : radius(radius) {}

BoxShape::BoxShape(double width, double height, double depth) : half_size(Vec3(width, height, depth) * 0.5) {}

BoxShape::BoxShape(double size) : half_size(Vec3(size, size, size) * 0.5) {}

Vec3 BoxShape::GetSupportingVector(const Vec3 &dir) const {
    return Sign(dir) * half_size;
}

SumShape::SumShape(std::unique_ptr<ConvexShape> a, std::unique_ptr<ConvexShape> b)
        : a(std::move(a)), b(std::move(b)) {}

Vec3 SumShape::GetSupportingVector(const Vec3 &dir) const {
    return a->GetSupportingVector(dir) + b->GetSupportingVector(dir);
}
