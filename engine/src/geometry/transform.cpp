//
// Created by User on 8/28/2020.
//

#include <engine/geometry/transform.hpp>

Transform::Transform(const Vec3 &translation, const Quat &rotation)
        : translation(translation), rotation(rotation), rotation_mat(rotation.Mat()) {}

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
    rotation = rotation_.Norm();
    rotation_mat = rotation.Mat();
    inv_rotation_mat = rotation_mat.T();
    TransformUpdated();
}

void Transform::Translate(const Vec3 &translation_) {
    translation += translation_;
    TransformUpdated();
}

void Transform::TransformUpdated() {}

const Mat3 &Transform::GetRotationMat() const {
    return rotation_mat;
}

const Mat3 &Transform::GetInvRotationMat() const {
    return inv_rotation_mat;
}
