#pragma once

#include <vector>
#include <tuple>
#include <engine/collision/bounding_volume.hpp>
#include <engine/physics/physics_engine.hpp>

typedef std::tuple<int,int,int> Triple;

std::vector<Triple> GetBoxFaces();

std::vector<Vec3> GetBoxVertices(double w, double h, double d);

std::vector<Vec3> GetBoxVertices(const BoundingBox &bbox);

PhysicsEngine LoadSampleScene();