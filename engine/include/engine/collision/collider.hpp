#pragma once

#include <memory>
#include <common/math/quat.hpp>
#include <common/transformable.hpp>
#include "bounding_volume.hpp"
#include "shape.hpp"

class Collider : public ConvexShape, public Transformable {
public:
    ~Collider() override = default;

    Collider(std::unique_ptr<ConvexShape> shape);

    Vec3 GetSupportingVector(const Vec3 &dir) const override;

    const BoundingBox &GetBBox() const;

    const Vec3 &GetTranslation() const override;

    const Quat &GetRotation() const override;

    double GetScale() const override;

    void SetTranslation(const Vec3 &translation) override;

    void SetRotation(const Quat &rotation) override;

    void SetScale(double scale) override;

    void Translate(const Vec3 &translation) override;

    void Rotate(const Quat &rotation) override;

    void Scale(double scale) override;

    // TODO: DEPRECATED!
    const ConvexShape* GetOrigShape() const;

private:

    void TransformUpdated();

    std::unique_ptr<ConvexShape> shape;

    BoundingBox bbox;

    Vec3 translation;
    Quat rotation;
};

std::unique_ptr<Collider> CreateBoxCollider(double size, Vec3 pos = Vec3(), Quat rot = Quat::Identity());

std::unique_ptr<Collider> CreateBoxCollider(Vec3 size, Vec3 pos = Vec3(), Quat rot = Quat::Identity());

std::unique_ptr<Collider> CreateSphereCollider(double r, Vec3 pos = Vec3(), Quat rot = Quat::Identity());
