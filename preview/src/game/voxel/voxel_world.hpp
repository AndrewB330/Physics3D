#pragma once

#include <map>
#include <memory>
#include "voxel_object.hpp"

const double VOXEL_SIZE = 0.1;
const Vec3i GROUND_DIMS = {256, 128, 256};

const uint8_t SHADOW_FACTOR = 2;


class VoxelWorld {
public:
    VoxelWorld();

    void AddVoxelObject(std::shared_ptr<VoxelObject> object);

    VoxelObject& GetGroundObject();

    VoxelObject& GetShadowObject();

    const std::vector<std::shared_ptr<VoxelObject>> & GetObjects();

    void GroundUpdated();

    void ObjectUpdated(int id);

private:
    std::vector<std::shared_ptr<VoxelObject>> objects;
    std::map<int, std::shared_ptr<VoxelObject>> object_by_id;
    VoxelObject ground;
    VoxelObject shadow;
};