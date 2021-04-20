#include "voxel_world.hpp"

VoxelWorld::VoxelWorld()
        : ground(GROUND_DIMS),
          shadow(GROUND_DIMS * SHADOW_FACTOR, false){
    Vec3 center = Vec3(GROUND_DIMS.x, 0, GROUND_DIMS.z) * -0.5 * VOXEL_SIZE;
    ground.SetScale(VOXEL_SIZE);
    shadow.SetScale(VOXEL_SIZE / SHADOW_FACTOR);
    ground.SetTranslation(center);
    shadow.SetTranslation(center);
}

void VoxelWorld::AddVoxelObject(std::shared_ptr<VoxelObject> object) {
    object_by_id[object->GetId()] = object;
    objects.emplace_back(std::move(object));
}

VoxelObject &VoxelWorld::GetGroundObject() {
    return ground;
}

VoxelObject &VoxelWorld::GetShadowObject() {
    return shadow;
}


const std::vector<std::shared_ptr<VoxelObject>> &VoxelWorld::GetObjects() {
    return objects;
}

void VoxelWorld::GroundUpdated() {
    for(int i = 0; i < GROUND_DIMS.x * 2; i++) {
        for(int j = 0; j < GROUND_DIMS.y * 2; j++) {
            for(int k = 0; k < GROUND_DIMS.z * 2; k++) {
                shadow.SetVoxelSilent(i, j, k, ground.GetVoxel(i >> 1, j >> 1, k >> 1));
            }
        }
    }
    shadow.Finalize();
}

void VoxelWorld::ObjectUpdated(int id) {
    if (object_by_id.find(id) == object_by_id.end())
        return;

    Vec3 min = Vec3(1e9, 1e9, 1e9);
    Vec3 max = -min;

    const auto & object = object_by_id[id];
    auto [w, h, d] = object->GetDims();

    for(int i = 0; i < 8; i++) {
        Vec3 pos = Vec3(w * (i & 1), h * ((i & 2) >> 1), d * ((i & 4) >> 2));
        pos = shadow.ApplyTransformInv(object->ApplyTransform(pos));
        min = Vec3::Min(min, pos);
        max = Vec3::Max(max, pos);
    }

    min = Vec3::Max(min, Vec3());
    max = Vec3::Min(max, Vec3(shadow.GetDims().x, shadow.GetDims().y, shadow.GetDims().z));

    for(int i = min.x; i < max.x; i++) {
        for(int j = min.y; j < max.y; j++) {
            for(int k = min.z; k < max.z; k++) {
                bool shaded = true;
                for(int m = 0; m < 8; m++) {
                    Vec3 pos = 0.3 * (Vec3((m & 1), ((m & 2) >> 1), ((m & 4) >> 2)) * 2 - Vec3(1, 1, 1)) +
                            Vec3(i + 0.5, j + 0.5, k + 0.5);
                    pos = object->ApplyTransformInv(shadow.ApplyTransform(pos));
                    shaded &= object->GetVoxel(floor(pos.x), floor(pos.y), floor(pos.z)) > 0;
                }
                shadow.SetVoxelSilent(i, j, k, shaded);
            }
        }
    }
    shadow.Finalize();
}
