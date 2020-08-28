#pragma once

#include <math/vec3.hpp>
#include <math/quat.hpp>

class Transform {
public:
    Transform() = default;
    virtual ~Transform() = default;

    Transform(const Vec3 &translation, const Quat &rotation);

    const Vec3 &GetTranslation() const;

    const Quat &GetRotation() const;

    void SetTranslation(const Vec3 &translation);

    void SetRotation(const Quat &rotation);

    void Translate(const Vec3 & translation);

protected:
    virtual void TransformUpdated();

    Vec3 translation = Vec3();
    Quat rotation = Quat::Identity();
};