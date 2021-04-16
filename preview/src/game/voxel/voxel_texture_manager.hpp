#pragma once

#include "voxel_object.hpp"
#include "../../algorithms/fenwick_tree.hpp"
#include <map>
#include <GL/glew.h>
#include <queue>
#include <math/vec3i.hpp>

const int VOXEL_TEXTURE = 128;

const Vec3i TEXTURE_DIMS = Vec3i(256, 256, 256);

const uint32_t CHUNK_SIZE = 32;

class TextureAllocator {
public:
    TextureAllocator(const Vec3i & texture_dims, int additional_layers = CHUNK_SIZE);

    Vec3i Allocate(const Vec3i & dims);

    void Free(const Vec3i & offset);

private:
    uint32_t w, h, d;
    uint32_t additional_h;
    FenwickTreeRange3D chunk_used;
    FenwickTreeRange3D voxel_used;
    std::map<Vec3i, Vec3i> allocated;
};

class VoxelTextureManager {
public:
    VoxelTextureManager();

    void Init();

    void RegisterVoxelObject(const VoxelObject<uint8_t> &object);

    std::vector<Vec3i> GetTextureOffsets(int object_id);

    void check();

private:
    void UpdateTexture(const Vec3i & offset, Vec3i dims, const uint8_t * data);

    TextureAllocator allocator;

    GLuint gl_texture_id;

    std::map<int, std::vector<Vec3i>> texture_offsets;
};
