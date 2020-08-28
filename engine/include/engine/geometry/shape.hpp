#pragma once

#include <memory>

#include <math/vec3.hpp>
#include <math/quat.hpp>
#include <engine/geometry/utils.hpp>
#include <engine/geometry/transform.hpp>
#include <engine/geometry/bounding_box.hpp>

class Shape : public Transform {
public:
    virtual ~Shape() = default;

    const BoundingBox &GetBBox() const;

protected:
    BoundingBox bbox;
};

class ConvexShape : public Shape {
public:
    ~ConvexShape() override = default;

    virtual Vec3 GetSupportingVector(const Vec3 &direction) const = 0;

    virtual bool IsPointInside(const Vec3& point) const = 0;
};

class SphereShape : public ConvexShape {
public:
    ~SphereShape() override = default;

    explicit SphereShape(double radius);

    Vec3 GetSupportingVector(const Vec3 &direction) const override;

    bool IsPointInside(const Vec3& point) const override;

    double GetRadius() const;

protected:
    void TransformUpdated() override;

    double radius;
};

class ConvexPolyhedronShape : public ConvexShape {
public:
    ~ConvexPolyhedronShape() override = default;

    ConvexPolyhedronShape(std::vector<Vec3> vertices, std::vector<Triple> faces);

    Vec3 GetSupportingVector(const Vec3 &direction) const override;

    bool IsPointInside(const Vec3& point) const override;

    const std::vector<Vec3>& GetVertices() const;

    const std::vector<Triple>& GetFaces() const;

protected:
    void TransformUpdated() override;

    std::vector<Vec3> vertices;
    std::vector<Triple> faces;

    std::vector<Vec3> transformed_vertices;
};

class BoxShape : public ConvexPolyhedronShape {
public:
    ~BoxShape() override = default;

    explicit BoxShape(double size);

    BoxShape(double width, double height, double depth);

    Vec3 GetSize() const;

private:
    double width, height, depth;
};

class MinkowskiDifferenceShape : public ConvexShape {
public:
    ~MinkowskiDifferenceShape() override = default;

    MinkowskiDifferenceShape(const ConvexShape *a, const ConvexShape *b);

    Vec3 GetSupportingVector(const Vec3 &direction) const override;

    bool IsPointInside(const Vec3& point) const override;

private:
    const ConvexShape *a;
    const ConvexShape *b;
};