#pragma once
#include <vector>
#include <engine/model/object.hpp>

struct Face {
    int v0, v1, v2;
};

class Polyhedron : public Object {
public:
    virtual ~Polyhedron() = default;

    Polyhedron(std::vector<Vec3> vertices, std::vector<Face> faces);

    const std::vector<Face>& GetFaces() const;
    const std::vector<Vec3>& GetVertices() const;
    const std::vector<Vec3>& GetNormals() const;

    virtual Vec3 GetSupportingVector(Vec3 direction) const override;

protected:
    virtual void TransformationUpdated() override;

    virtual void RecalculateInertia() override;

    std::vector<Vec3> vertices_transformed;
    std::vector<Vec3> vertices;

    std::vector<Vec3> normals_transformed;
    std::vector<Vec3> normals;

    std::vector<Face> faces;
};