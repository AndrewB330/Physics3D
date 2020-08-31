#pragma once

#include <math/vec3.hpp>
#include <memory>
#include <engine/geometry/transform.hpp>
#include "bounding_volume.hpp"

class Shape {
public:
    virtual ~Shape() = default;
};

class ConvexShape : public Shape {
public:
    ~ConvexShape() override = default;

    virtual Vec3 GetSupportingVector(const Vec3 &dir) const;
};

class SphereShape : public ConvexShape {
public:
    ~SphereShape() override = default;

    SphereShape(double radius);

    Vec3 GetSupportingVector(const Vec3 &dir) const override;

private:
    double radius;
};

class BoxShape : public ConvexShape {
public:
    ~BoxShape() override = default;

    BoxShape(double width, double height, double depth);

    explicit BoxShape(double size);

    Vec3 GetSupportingVector(const Vec3 &dir) const override;

private:
    Vec3 half_size;
};

class SumShape : public ConvexShape {
public:
    ~SumShape() override = default;

    SumShape(std::unique_ptr<ConvexShape> a, std::unique_ptr<ConvexShape> b);

    Vec3 GetSupportingVector(const Vec3 &dir) const override;

private:
    std::unique_ptr<ConvexShape> a;
    std::unique_ptr<ConvexShape> b;
};

class Collider : public ConvexShape, public Transform {
public:
    ~Collider() override = default;

    Collider(std::unique_ptr<ConvexShape> shape);

    Vec3 GetSupportingVector(const Vec3 &dir) const override;

    const BoundingBox &GetBBox() const;

    // TODO: DEPERECATED!
    const ConvexShape* GetOrigShape() const;

private:
    void TransformUpdated() override;

    std::unique_ptr<ConvexShape> shape;

    BoundingBox bbox;
};

std::unique_ptr<Collider> CreateBoxCollider(double size, Vec3 pos = Vec3(), Quat rot = Quat::Identity());

std::unique_ptr<Collider> CreateBoxCollider(Vec3 size, Vec3 pos = Vec3(), Quat rot = Quat::Identity());

std::unique_ptr<Collider> CreateSphereCollider(double r, Vec3 pos = Vec3(), Quat rot = Quat::Identity());
