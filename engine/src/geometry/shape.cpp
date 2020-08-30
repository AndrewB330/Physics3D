#include <engine/geometry/shape.hpp>

const BoundingBox &Shape::GetBBox() const {
    return bbox;
}

Vec3 SphereShape::GetSupportingVector(const Vec3 &direction) const {
    return translation + direction.Norm() * radius;
}

double SphereShape::GetRadius() const {
    return radius;
}

SphereShape::SphereShape(double radius) : radius(radius) {
    SphereShape::TransformUpdated();
}

void SphereShape::TransformUpdated() {
    bbox.max.x = translation.x + radius;
    bbox.max.y = translation.y + radius;
    bbox.max.z = translation.z + radius;
    bbox.min.x = translation.x - radius;
    bbox.min.y = translation.y - radius;
    bbox.min.z = translation.z - radius;
}

bool SphereShape::IsPointInside(const Vec3 &point) const {
    return (point - translation).LenSqr() <= radius * radius;
}

ConvexPolyhedronShape::ConvexPolyhedronShape(std::vector<Vec3> vertices, std::vector<Triple> faces)
        : vertices(std::move(vertices)), faces(std::move(faces)) {
    ConvexPolyhedronShape::TransformUpdated();
}

Vec3 ConvexPolyhedronShape::GetSupportingVector(const Vec3 &direction) const {
    Vec3 best_vertex = transformed_vertices[0];
    double best_projection = Dot(direction, transformed_vertices[0]);
    for (int i = 1; i < transformed_vertices.size(); i++) {
        double projection = Dot(direction, transformed_vertices[i]);
        if (projection > best_projection) {
            best_projection = projection;
            best_vertex = transformed_vertices[i];
        }
    }
    return best_vertex;
}

void ConvexPolyhedronShape::TransformUpdated() {
    Mat3 rotation_mat = rotation.Mat();

    transformed_vertices.resize(vertices.size());
    for (int i = 0; i < vertices.size(); i++) {
        transformed_vertices[i] = translation + rotation_mat * vertices[i];
    }

    bbox = BoundingBox();
    for (const auto &v : transformed_vertices) {
        bbox.min.x = std::min(bbox.min.x, v.x);
        bbox.min.y = std::min(bbox.min.y, v.y);
        bbox.min.z = std::min(bbox.min.z, v.z);

        bbox.max.x = std::max(bbox.max.x, v.x);
        bbox.max.y = std::max(bbox.max.y, v.y);
        bbox.max.z = std::max(bbox.max.z, v.z);
    }
}

const std::vector<Vec3> &ConvexPolyhedronShape::GetVertices() const {
    return transformed_vertices;
}

const std::vector<Triple> &ConvexPolyhedronShape::GetFaces() const {
    return faces;
}

bool ConvexPolyhedronShape::IsPointInside(const Vec3 &point) const {
    for (auto[v0, v1, v2] : faces) {
        double volume = Dot(
                point - transformed_vertices[v0],
                Cross(
                        transformed_vertices[v1] -
                        transformed_vertices[v0],
                        transformed_vertices[v2] -
                        transformed_vertices[v0]
                ));
        if (volume > 1e-6) {
            return false;
        }
    }
    return true;
}

BoxShape::BoxShape(double size)
        : ConvexPolyhedronShape(GetBoxVertices(size, size, size), GetBoxFaces()),
          width(size), height(size), depth(size) {
    BoxShape::TransformUpdated();
}

BoxShape::BoxShape(double width, double height, double depth)
        : ConvexPolyhedronShape(GetBoxVertices(width, height, depth), GetBoxFaces()),
          width(width), height(height), depth(depth) {
    BoxShape::TransformUpdated();
}

Vec3 BoxShape::GetSize() const {
    return Vec3(width, height, depth);
}


Vec3 MinkowskiDifferenceShape::GetSupportingVector(const Vec3 &direction) const {
    return a->GetSupportingVector(direction) - b->GetSupportingVector(-direction);
}

MinkowskiDifferenceShape::MinkowskiDifferenceShape(const ConvexShape *a, const ConvexShape *b)
        : a(a), b(b) {}

bool MinkowskiDifferenceShape::IsPointInside(const Vec3 &point) const {
    // TODO: THIS IS NOT IMPLEMENTED FOR MINKOWSKI
    return false;
}
