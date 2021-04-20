#pragma once

#include <common/transform.hpp>
#include <math/vec3i.hpp>

const uint8_t LOD_NUM = 7; // 0...7 lod | 1 ... 128 voxels

/**
 * VoxelMesh - data structure to represent 3 dimensional grid of "voxels".
 * Supports compressed LODs (level of details).
 */
class VoxelObject : public Transform {
public:
    VoxelObject(const VoxelObject &) = delete;

    VoxelObject(uint32_t w = 1, uint32_t h = 1, uint32_t d = 1, bool has_data = true, uint8_t fill = 0);

    VoxelObject(Vec3i dims, bool hasData = true, T fill = 0);

    virtual ~VoxelObject();

    /**
     * Returns voxel data at position (x, y, z)
     */
    uint8_t GetVoxel(int32_t x, int32_t y, int32_t z) const;

    /**
     * Change voxel data at position (x, y, z) and update lod-s
     */
    //void SetVoxel(int32_t x, int32_t y, int32_t z, uint8_t value);

    /**
     * Change voxel data at position (x, y, z) without lod update, should call Finalize afterwards
     */
    void SetVoxelSilent(int32_t x, int32_t y, int32_t z, T value);

    /**
     * Update all lod-s
     */
    void Finalize();

    /**
     * Returns dims (width, height, depth) of the grid for the given lod
     */
    Vec3i GetLodDims(uint8_t lod = 0) const;

    /**
     * Returns dims (width, height, depth)
     */
    Vec3i GetDims() const;

    /**
     * Returns smallest lod that has dims (1, 1, 1)
     */
    uint8_t GetFirstFullLod() const;

    /**
     * Returns a pointer to data for a given lod
     */
    const uint8_t *GetLodDataPointer(uint8_t lod) const;

    const uint8_t *GetDataPointer() const;

    bool HasData() const;

    int GetId() const;

protected:
    /**
     * Convert 3D coordinates to 1D array index (for a given lod)
     */
    int32_t GetArrayIndex(int32_t x, int32_t y, int32_t z, uint8_t lod) const;

    uint8_t GetBitIndex(int32_t x, int32_t y, int32_t z) const;

    bool HasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod) const;

    void SetHasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod, bool has);

    /**
     * Update all lod-s
     */
    void RecalculateLod();

    /// Dimensions
    uint32_t w = 1, h = 1, d = 1;

    /// Linearized and compressed lod data (data[i] - i-th lod)
    std::vector<uint8_t> lodData[LOD_NUM];
    ///
    std::vector<uint8_t> data;

    /// True if there were any silent updates
    bool silent_updates = false;
    ///
    bool has_data;
    /// Unique identifier
    int id;
};
