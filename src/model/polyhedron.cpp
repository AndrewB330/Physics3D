#include "polyhedron.h"

Polyhedron::Polyhedron(std::vector<Vec3> vertices, std::vector<Face> faces) :Object(), vertices(vertices), faces(faces) {
    for (auto& f : this->faces) {
        f.normal = Cross(vertices[f.v1] - vertices[f.v0], vertices[f.v2] - vertices[f.v0]).Norm();
    }

    TransformationUpdated();
}

const std::vector<Face>& Polyhedron::GetFaces() const {
    return faces;
}

const std::vector<Vec3>& Polyhedron::GetVertices() const {
    return vertices_transformed;
}

Vec3 Polyhedron::GetSupportingPoint(Vec3 direction) const {
    Vec3 supporting_point = vertices_transformed[0];
    double max_dot = std::numeric_limits<double>::lowest();
    for (const auto& v : vertices_transformed) {
        double dot = Dot(v, direction);
        if (dot > max_dot) {
            max_dot = dot;
            supporting_point = v;
        }
    }
    return supporting_point;
}

void Polyhedron::TransformationUpdated() {
    bbox = BoundingBox();
    for (const auto& v : vertices_transformed) {
        bbox.min.x = std::min(bbox.min.x, v.x);
        bbox.min.y = std::min(bbox.min.y, v.y);
        bbox.min.z = std::min(bbox.min.z, v.z);

        bbox.max.x = std::max(bbox.max.x, v.x);
        bbox.max.y = std::max(bbox.max.y, v.y);
        bbox.max.z = std::max(bbox.max.z, v.z);
    }

    vertices_transformed = vertices;
    auto mat = state.rotation.Mat();
    for (auto & v : vertices_transformed) {
        v = state.position + mat * v;
    }
}

void Polyhedron::RecalculateInertia() {
    if (fixed) {
        inertia = ObjectInertia();
    } else {
        inertia.mass = 1.0;
        inertia.mass_inv = 1.0;
        //todo: calculate!!!!
    }
}

Face::Face(int v0, int v1, int v2) :v0(v0), v1(v1), v2(v2) {}

Face::Face() : v0(0), v1(0), v2(0) {}
