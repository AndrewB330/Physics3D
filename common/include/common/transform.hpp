#pragma once

#include <math/quat.hpp>

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

    virtual void Scale(double factor) = 0;
};

class Transform : public Transformable {
public:
    Transform(const Vec3 & translation = Vec3(), const Quat & rotation = Quat::Identity());

    ~Transform() override;

    const Vec3 &GetTranslation() const override;

    const Quat &GetRotation() const override;

    double GetScale() const override;

    void SetTranslation(const Vec3 &translation) override;

    void SetRotation(const Quat &rotation) override;

    void SetScale(double scale) override;

    void Translate(const Vec3 &translation) override;

    void Rotate(const Quat &rotation) override;

    void Scale(double factor) override;

    virtual void TransformUpdated();

    Vec3 ApplyTransform(Vec3 point) const;

    Vec3 ApplyTransformInv(Vec3 point) const;

protected:
    Vec3 translation = Vec3();
    Quat rotation = Quat::Identity();
    double scale = 1.0;
};