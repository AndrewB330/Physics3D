#pragma once

#include <memory>
#include "voxel_object.hpp"

const double WORLD_VOXEL_SIZE = 0.1;
const Vec3i GROUND_DIMS = {256, 128, 256};

class VoxelWorld {
public:
    VoxelWorld();

    void AddVoxelObject(std::shared_ptr<VoxelObject<uint8_t>> object);

    VoxelObject<uint8_t>& GetGroundObject();

    const std::vector<std::shared_ptr<VoxelObject<uint8_t>>> & GetObjects();

private:
    std::vector<std::shared_ptr<VoxelObject<uint8_t>>> objects;
    VoxelObject<uint8_t> ground;
};