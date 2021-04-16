#pragma once

#include <math/quat.hpp>

class Transformable {
public:
    virtual ~Transformable() = default;

    Transformable() = default;

    virtual const Vec3 &GetTranslation() const = 0;

    virtual const Quat &GetRotation() const = 0;

    virtual void SetTranslation(const Vec3 &translation) = 0;

    virtual void SetRotation(const Quat &rotation) = 0;

    virtual void Translate(const Vec3 &translation) = 0;

    virtual void Rotate(const Quat &rotation) = 0;
};

class Transform : public Transformable {
public:
    Transform(const Vec3 & translation = Vec3(), const Quat & rotation = Quat::Identity());

    ~Transform() override;

    const Vec3 &GetTranslation() const override;

    const Quat &GetRotation() const override;

    void SetTranslation(const Vec3 &translation) override;

    void SetRotation(const Quat &rotation) override;

    void Translate(const Vec3 &translation) override;

    void Rotate(const Quat &rotation) override;

    virtual void TransformUpdated();

protected:
    Vec3 translation;
    Quat rotation;
};