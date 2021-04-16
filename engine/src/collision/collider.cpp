#include <engine/collision/collider.hpp>

Collider::Collider(std::unique_ptr<ConvexShape> shape) : shape(std::move(shape)) {
    Collider::TransformUpdated();
}

Vec3 Collider::GetSupportingVector(const Vec3 &dir) const {
    return translation + rotation.ApplyRotation(shape->GetSupportingVector(rotation.ApplyInvRotation(dir)));
}

void Collider::TransformUpdated() {
    bbox.min.x = GetSupportingVector(Vec3(-1, 0, 0)).x;
    bbox.max.x = GetSupportingVector(Vec3(1, 0, 0)).x;

    bbox.min.y = GetSupportingVector(Vec3(0, -1, 0)).y;
    bbox.max.y = GetSupportingVector(Vec3(0, 1, 0)).y;

    bbox.min.z = GetSupportingVector(Vec3(0, 0, -1)).z;
    bbox.max.z = GetSupportingVector(Vec3(0, 0, 1)).z;
}

const BoundingBox &Collider::GetBBox() const {
    return bbox;
}

const ConvexShape *Collider::GetOrigShape() const {
    return shape.get();
}

std::unique_ptr<Collider> CreateBoxCollider(double size, Vec3 pos, Quat rot) {
    auto shape = std::make_unique<BoxShape>(size);
    auto collider = std::make_unique<Collider>(std::move(shape));
    collider->SetTranslation(pos);
    collider->SetRotation(rot);
    return collider;
}

std::unique_ptr<Collider> CreateBoxCollider(Vec3 size, Vec3 pos, Quat rot) {
    auto shape = std::make_unique<BoxShape>(size.x, size.y, size.z);
    auto collider = std::make_unique<Collider>(std::move(shape));
    collider->SetTranslation(pos);
    collider->SetRotation(rot);
    return collider;
}

std::unique_ptr<Collider> CreateSphereCollider(double r, Vec3 pos, Quat rot) {
    auto shape = std::make_unique<SphereShape>(r);
    auto collider = std::make_unique<Collider>(std::move(shape));
    collider->SetTranslation(pos);
    collider->SetRotation(rot);
    return collider;
}