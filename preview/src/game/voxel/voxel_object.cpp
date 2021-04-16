//
// Created by User on 3/30/2021.
//

#include <common/bit_utils.hpp>
#include <tuple>
#include <common/assert.hpp>
#include "voxel_object.hpp"

template<typename T>
VoxelMesh<T>::VoxelMesh(uint32_t w_, uint32_t h_, uint32_t d_, T fill)
        : w(NextPowerOfTwo(w_)), h(NextPowerOfTwo(h_)), d(NextPowerOfTwo(d_)) {
    data[0].assign(GetCompressedWidth() * GetCompressedHeight() * GetCompressedDepth(),
                   (IsCompressed(0) ? 0b111 : fill));
    RecalculateLod();
}

template<typename T>
VoxelMesh<T>::~VoxelMesh() {}

template<typename T>
uint32_t VoxelMesh<T>::GetWidth(uint8_t lod) const {
    return std::max(IsCompressed(lod) ? 2u : 1u, w >> lod);
}

template<typename T>
uint32_t VoxelMesh<T>::GetHeight(uint8_t lod) const {
    return std::max(IsCompressed(lod) ? 2u : 1u, h >> lod);
}

template<typename T>
uint32_t VoxelMesh<T>::GetDepth(uint8_t lod) const {
    return std::max(IsCompressed(lod) ? 2u : 1u, d >> lod);
}

template<typename T>
Vec3i VoxelMesh<T>::GetGridDims(uint8_t lod) const {
    return Vec3i(GetWidth(lod), GetHeight(lod), GetDepth(lod));
}

template<typename T>
void VoxelMesh<T>::RecalculateLod() {
    for (uint8_t lod = 1; lod < LOD_NUM; lod++) {
        auto[prev_w, prev_h, prev_d] = GetGridDims(lod - 1);

        auto[c_w, c_h, c_d] = GetCompressedGridDims(lod);
        data[lod].assign(c_w * c_h * c_d, 0);

        for (int32_t x = 0; x < prev_w; x++) {
            for (int32_t y = 0; y < prev_h; y++) {
                for (int32_t z = 0; z < prev_d; z++) {
                    if (HasVoxel(x, y, z, lod-1)) {
                        SetHasVoxel(x >> 1, y >> 1, z >> 1, lod, 1);
                    }
                }
            }
        }
        /*if (lod) {
            data[lod].assign(data[lod].size(), 255);
        }*/
    }
}

template<typename T>
int32_t VoxelMesh<T>::GetArrayIndex(int32_t x, int32_t y, int32_t z, uint8_t lod) const {
    if (IsCompressed(lod)) {
        return (x >> 1) +
               (y >> 1) * GetCompressedWidth(lod) +
               (z >> 1) * GetCompressedWidth(lod) * GetCompressedHeight(lod);
    } else {
        return x + y * GetWidth(lod) + z * GetWidth(lod) * GetHeight(lod);
    }
}

template<typename T>
uint32_t VoxelMesh<T>::GetCompressedWidth(uint8_t lod) const {
    if (IsCompressed(lod)) {
        return std::max(1u, w >> (lod + 1));
    }
    return w;
}

template<typename T>
uint32_t VoxelMesh<T>::GetCompressedHeight(uint8_t lod) const {
    if (IsCompressed(lod)) {
        return std::max(1u, h >> (lod + 1));
    }
    return h;
}

template<typename T>
uint32_t VoxelMesh<T>::GetCompressedDepth(uint8_t lod) const {
    if (IsCompressed(lod)) {
        return std::max(1u, d >> (lod + 1));
    }
    return d;
}

template<typename T>
Vec3i VoxelMesh<T>::GetCompressedGridDims(uint8_t lod) const {
    return Vec3i(GetCompressedWidth(lod), GetCompressedHeight(lod), GetCompressedDepth(lod));
}

template<typename T>
T VoxelMesh<T>::GetVoxel(int32_t x, int32_t y, int32_t z) const {
    if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d) {
        return 0;
    }
    if (std::is_same<T, bool>::value)
        return HasVoxel(x, y, z, 0);
    return data[0].at(GetArrayIndex(x, y, z, 0));
}

template<typename T>
uint8_t VoxelMesh<T>::GetFirstFullLod() const {
    uint8_t res = 1;
    for (uint8_t i = 0; i < LOD_NUM; i++) {
        auto[w, h, d] = GetGridDims(i);
        if (w * h * d > 1) res = i + 1;
    }
    return res;
}

template<typename T>
void VoxelMesh<T>::SetVoxel(int32_t x, int32_t y, int32_t z, T value) {
    if (silent_updates) {
        RecalculateLod();
    }

    if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d) {
        return;
    }

    if (std::is_same<T, bool>::value) {
        SetHasVoxel(x, y, z, 0, value);
    } else {
        data[0][GetArrayIndex(x, y, z, 0)] = value;
    }

    if (value) {
        for (uint8_t lod = 1; lod < LOD_NUM; lod++) {
            x >>= 1;
            y >>= 1;
            z >>= 1;
            SetHasVoxel(x, y, z, lod, true);
        }
    } else {
        for (uint8_t lod = 1; lod < LOD_NUM; lod++) {
            int32_t x_c = (x >> 1) << 1;
            int32_t y_c = (y >> 1) << 1;
            int32_t z_c = (z >> 1) << 1;
            bool any = false;
            for (int dx = 0; dx < 2; dx++) {
                for (int dy = 0; dy < 2; dy++) {
                    for (int dz = 0; dz < 2; dz++) {
                        any |= HasVoxel(x_c + dx, y_c + dy, z_c + dz, lod - 1);
                    }
                }
            }
            if (any) {
                break;
            }
            x >>= 1;
            y >>= 1;
            z >>= 1;
            SetHasVoxel(x, y, z, lod, false);
        }
    }
}

template<typename T>
void VoxelMesh<T>::SetVoxelSilent(int32_t x, int32_t y, int32_t z, T value) {
    silent_updates = true;
    if (std::is_same<T, bool>::value) {
        SetHasVoxel(x, y, z, 0, value);
    } else {
        data[0][GetArrayIndex(x, y, z, 0)] = value;
    }
}

template<typename T>
const uint8_t *VoxelMesh<T>::GetDataPointer(uint8_t lod) const {
    return data[lod].data();
}

template<typename T>
void VoxelMesh<T>::Finalize() {
    RecalculateLod();
}

template<typename T>
VoxelMesh<T>::VoxelMesh(Vec3i dims, T fill) : VoxelMesh(dims.x, dims.y, dims.z, fill) {}

template<typename T>
uint8_t VoxelMesh<T>::GetBitIndex(int32_t x, int32_t y, int32_t z) const {
    return (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
}

template<typename T>
bool VoxelMesh<T>::HasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod) const {
    if (IsCompressed(lod)) {
        return (data[lod][GetArrayIndex(x, y, z, lod)] >> GetBitIndex(x, y, z)) & 1;
    } else {
        return data[lod][GetArrayIndex(x, y, z, lod)];
    }
}

template<typename T>
void VoxelMesh<T>::SetHasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod, bool has) {
    ENSURE_THAT(lod > 0);
    if (lod == 0) return;

    if (IsCompressed(lod)) {
        int32_t index = GetArrayIndex(x, y, z, lod);
        uint8_t bit = GetBitIndex(x, y, z);
        data[lod][index] &= ~(1u << bit);
        data[lod][index] |= (has << bit);
    } else {
        data[lod][GetArrayIndex(x, y, z, lod)] = has;
    }
}

template<typename T>
bool VoxelMesh<T>::IsCompressed(uint8_t lod) const {
    return lod || std::is_same<T, bool>::value;
}

template<typename T>
VoxelObject<T>::VoxelObject(double voxel_size, uint32_t w, uint32_t h, uint32_t d, T fill)
        : VoxelMesh<T>(w, h, d, fill), voxel_size(voxel_size), id(nextId++) {}

template<typename T>
VoxelObject<T>::VoxelObject(double voxel_size, Vec3i dims, T fill)
        : VoxelObject(voxel_size, dims.x, dims.y, dims.z, fill) {}

template<typename T>
double VoxelObject<T>::GetVoxelSize() const {
    return voxel_size;
}

template<typename T>
VoxelObject<T>::~VoxelObject() {}

template<typename T>
int VoxelObject<T>::nextId = 1;

template<typename T>
int VoxelObject<T>::GetId() const {
    return id;
}