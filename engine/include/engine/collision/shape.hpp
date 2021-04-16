#pragma once

#include <memory>
#include <math/vec3.hpp>

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
