#pragma once
#include <vector>
#include "object.h"

struct Face {
    int v0, v1, v2;
    Vec3 normal;

    Face(int v0, int v1, int v2);
    Face();
};

class Polyhedron : public Object {
public:
    virtual ~Polyhedron() = default;

    Polyhedron(std::vector<Vec3> vertices, std::vector<Face> faces);

    const std::vector<Face>& GetFaces() const;
    const std::vector<Vec3>& GetVertices() const;

    virtual Vec3 GetSupportingPoint(Vec3 direction) const override;

protected:
    virtual void TransformationUpdated() override;

    virtual void RecalculateInertia() override;

    std::vector<Vec3> vertices_transformed;
    std::vector<Vec3> vertices;
    std::vector<Face> faces;
};