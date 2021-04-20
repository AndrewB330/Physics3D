#include <common/bit_utils.hpp>
#include <tuple>
#include <common/assert.hpp>
#include "voxel_object.hpp"

const uint32_t MIN_SIDE = 4;

uint32_t nextId = 1;

VoxelObject::VoxelObject(uint32_t w_, uint32_t h_, uint32_t d_, bool has_data, uint8_t fill)
        : w(w_), h(h_), d(d_), has_data(has_data), id(nextId++) {
    if (has_data) {
        data.assign(w * h * d, fill);
    }
    for(int lod = 0; lod < LOD_NUM; lod++) {
        auto[ww, hh, dd] = GetLodDims(lod);
        lodData[lod].assign(ww * hh * dd, (fill ? 0b111 : 0b000));
    }
    RecalculateLod();
}

VoxelObject::VoxelObject(Vec3i dims, bool has_data, T fill) : VoxelObject(dims.x, dims.y, dims.z, has_data, fill) {}

VoxelObject::~VoxelObject() {}

Vec3i VoxelObject::GetLodDims(uint8_t lod) const {
    return Vec3i(
            std::max(MIN_SIDE, w >> (lod + 1)),
            std::max(MIN_SIDE, h >> (lod + 1)),
            std::max(MIN_SIDE, d >> (lod + 1))
    );
}

Vec3i VoxelObject::GetDims() const {
    return Vec3i(w, h, d);
}

void VoxelObject::RecalculateLod() {
    if (has_data) {
        auto[ww, hh, dd] = GetLodDims(0);
        lodData[0].assign(ww * hh * dd, 0);
        for (int32_t x = 0; x < w; x++) {
            for (int32_t y = 0; y < h; y++) {
                for (int32_t z = 0; z < d; z++) {
                    if (data.at(x + y * w + z * w * h))
                        SetHasVoxel(x, y, z, 0, 1);
                }
            }
        }
    }

    for (uint8_t lod = 1; lod < LOD_NUM; lod++) {
        auto[prev_w, prev_h, prev_d] = GetLodDims(lod - 1);
        auto[cur_w, cur_h, cur_d] = GetLodDims(lod);
        lodData[lod].assign(cur_w * cur_h * cur_d, 0);

        for (int32_t x = 0; x < prev_w; x++) {
            for (int32_t y = 0; y < prev_h; y++) {
                for (int32_t z = 0; z < prev_d; z++) {
                    if (lodData[lod - 1].at(x + y * prev_w + z * prev_w * prev_h)) {
                        SetHasVoxel(x, y, z, lod, 1);
                    }
                }
            }
        }
    }
}

uint8_t VoxelObject::GetVoxel(int32_t x, int32_t y, int32_t z) const {
    if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d) {
        return 0;
    }
    if (!has_data)
        return HasVoxel(x, y, z, 0);
    return data.at(x + y * w + z * w * h);
}

uint8_t VoxelObject::GetFirstFullLod() const {
    uint8_t res = 1;
    for (uint8_t i = 0; i < LOD_NUM; i++) {
        if ((w >> i) > 1 || (h >> i) > 1 || (d >> i) > 1)
            res = i + 1;
    }
    return res;
}

void VoxelObject::SetVoxelSilent(int32_t x, int32_t y, int32_t z, T value) {
    if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d) {
        return;
    }

    silent_updates = true;
    if (!has_data) {
        SetHasVoxel(x, y, z, 0, value);
    } else {
        data.at(x + y * w + z * w * h) = value;
    }
}

const uint8_t *VoxelObject::GetLodDataPointer(uint8_t lod) const {
    return lodData[lod].data();
}

void VoxelObject::Finalize() {
    RecalculateLod();
}

int32_t VoxelObject::GetArrayIndex(int32_t x, int32_t y, int32_t z, uint8_t lod) const {
    auto[ww, hh, dd] = GetLodDims(lod);
    return (x >> 1) + (y >> 1) * ww + (z >> 1) * ww * hh;
}

uint8_t VoxelObject::GetBitIndex(int32_t x, int32_t y, int32_t z) const {
    return (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
}

bool VoxelObject::HasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod) const {
    return (lodData[lod].at(GetArrayIndex(x, y, z, lod)) >> GetBitIndex(x, y, z)) & 1;
}

void VoxelObject::SetHasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod, bool has) {
    int32_t index = GetArrayIndex(x, y, z, lod);
    uint8_t bit = GetBitIndex(x, y, z);
    lodData[lod].at(index) &= ~(1u << bit);
    lodData[lod].at(index) |= (has << bit);
}

const uint8_t *VoxelObject::GetDataPointer() const {
    return data.data();
}

bool VoxelObject::HasData() const {
    return has_data;
}

int VoxelObject::GetId() const {
    return id;
}