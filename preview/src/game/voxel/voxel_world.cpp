#include "voxel_world.hpp"

VoxelWorld::VoxelWorld()
        : ground(WORLD_VOXEL_SIZE, GROUND_DIMS) {
    ground.SetTranslation(Vec3(GROUND_DIMS.x, 0, GROUND_DIMS.z) * -0.5 * WORLD_VOXEL_SIZE);
}

void VoxelWorld::AddVoxelObject(std::shared_ptr<VoxelObject<uint8_t>> object) {
    objects.emplace_back(std::move(object));
}

VoxelObject<uint8_t> &VoxelWorld::GetGroundObject() {
    return ground;
}

const std::vector<std::shared_ptr<VoxelObject<uint8_t>>> &VoxelWorld::GetObjects() {
    return objects;
}
