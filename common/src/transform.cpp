#include <common/transform.hpp>


Transform::Transform(const Vec3 &translation, const Quat &rotation)
        : translation(translation), rotation(rotation) {}

Transform::~Transform() {}

const Vec3 &Transform::GetTranslation() const {
    return translation;
}

const Quat &Transform::GetRotation() const {
    return rotation;
}

void Transform::SetTranslation(const Vec3 &translation_) {
    translation = translation_;
    TransformUpdated();
}

void Transform::SetRotation(const Quat &rotation_) {
    rotation = rotation_;
    TransformUpdated();
}

void Transform::Translate(const Vec3 &translation_) {
    translation += translation_;
    TransformUpdated();
}

void Transform::Rotate(const Quat &rotation_) {
    rotation = (rotation_ * rotation).Norm();
    TransformUpdated();
}

void Transform::TransformUpdated() {}

double Transform::GetScale() const {
    return scale;
}

void Transform::SetScale(double scale_) {
    scale = scale_;
    TransformUpdated();
}

void Transform::Scale(double factor) {
    scale *= scale;
    TransformUpdated();
}

Vec3 Transform::ApplyTransform(Vec3 point) const {
    return rotation.ApplyRotation(point * scale) + translation;
}

Vec3 Transform::ApplyTransformInv(Vec3 point) const {
    return rotation.ApplyInvRotation(point - translation) / scale;
}
