#pragma once

#include <vector>
#include <tuple>
#include <math/vec3.hpp>
#include <engine/collision/bounding_volume.hpp>

typedef std::tuple<int,int,int> Triple;

std::vector<Triple> GetBoxFaces();

std::vector<Vec3> GetBoxVertices(double w, double h, double d);

std::vector<Vec3> GetBoxVertices(const BoundingBox &bbox);
