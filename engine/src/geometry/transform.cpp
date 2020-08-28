//
// Created by User on 8/28/2020.
//

#include <engine/geometry/transform.hpp>

Transform::Transform(const Vec3 &translation, const Quat &rotation)
        : translation(translation), rotation(rotation) {}

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

void Transform::TransformUpdated() {}
