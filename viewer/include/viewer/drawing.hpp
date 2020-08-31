#pragma once

#include <engine/collision/collider.hpp>
#include <viewer/material.hpp>

namespace Graphics {
    //void DrawObject(const PhysObject* object);

    void DrawCollider(const Collider* shape, const RenderMaterial& material);

    void DrawBBox(const BoundingBox& bbox);
}
