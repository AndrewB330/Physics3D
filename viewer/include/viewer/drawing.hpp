#pragma once

#include <engine/geometry/shape.hpp>
#include <viewer/material.hpp>

namespace Graphics {
    //void DrawObject(const PhysObject* object);

    void DrawShape(const Shape* shape, const RenderMaterial& material);

    void DrawBBox(const BoundingBox& bbox);
}
