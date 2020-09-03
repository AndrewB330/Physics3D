#pragma once

#include <common/math/vec3.hpp>
#include <common/math/quat.hpp>

class Transformable {
public:
    virtual ~Transformable() = default;

    Transformable() = default;

    virtual const Vec3 &GetTranslation() const = 0;

    virtual const Quat &GetRotation() const = 0;

    virtual double GetScale() const = 0;

    virtual void SetTranslation(const Vec3 &translation) = 0;

    virtual void SetRotation(const Quat &rotation) = 0;

    virtual void SetScale(double scale) = 0;

    virtual void Translate(const Vec3 &translation) = 0;

    virtual void Rotate(const Quat &rotation) = 0;

    virtual void Scale(double scale) = 0;
};