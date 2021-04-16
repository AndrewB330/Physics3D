#pragma once

#include <memory>
#include <math/quat.hpp>
#include <common/transform.hpp>
#include "bounding_volume.hpp"
#include "shape.hpp"

class Collider : public ConvexShape, public Transform {
public:
    ~Collider() override = default;

    Collider(std::unique_ptr<ConvexShape> shape);

    Vec3 GetSupportingVector(const Vec3 &dir) const override;

    const BoundingBox &GetBBox() const;

    // TODO: DEPRECATED!
    const ConvexShape* GetOrigShape() const;

private:

    void TransformUpdated() override;

    std::unique_ptr<ConvexShape> shape;

    BoundingBox bbox;
};

std::unique_ptr<Collider> CreateBoxCollider(double size, Vec3 pos = Vec3(), Quat rot = Quat::Identity());

std::unique_ptr<Collider> CreateBoxCollider(Vec3 size, Vec3 pos = Vec3(), Quat rot = Quat::Identity());

std::unique_ptr<Collider> CreateSphereCollider(double r, Vec3 pos = Vec3(), Quat rot = Quat::Identity());
