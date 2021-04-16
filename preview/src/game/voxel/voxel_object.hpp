#pragma once

#include <common/transform.hpp>
#include <math/vec3i.hpp>

const uint8_t LOD_NUM = 8; // 0...7 lod | 1 ... 128 voxels

/**
 * VoxelMesh - data structure to represent 3 dimensional grid of "voxels".
 * Supports compressed LODs (level of details).
 */
template<typename T>
class VoxelMesh {
public:
    VoxelMesh(const VoxelMesh &) = delete;

    VoxelMesh(uint32_t w = 1, uint32_t h = 1, uint32_t d = 1, T fill = 0);

    VoxelMesh(Vec3i dims, T fill = 0);

    virtual ~VoxelMesh();

    /**
     * Returns voxel at position (x, y, z)
     */
    T GetVoxel(int32_t x, int32_t y, int32_t z) const;

    /**
     * Change voxel value at position (x, y, z) and update lod-s
     */
    void SetVoxel(int32_t x, int32_t y, int32_t z, T value);

    /**
     * Change voxel at position (x, y, z) without lod update, should call Finalize afterwards
     */
    void SetVoxelSilent(int32_t x, int32_t y, int32_t z, T value);

    /**
     * Update all lod-s
     */
    void Finalize();

    /**
     * Returns width of the grid for the given lod
     */
    uint32_t GetWidth(uint8_t lod = 0) const;

    /**
     * Returns height of the grid for the given lod
     */
    uint32_t GetHeight(uint8_t lod = 0) const;

    /**
     * Returns depth of the grid for the given lod
     */
    uint32_t GetDepth(uint8_t lod = 0) const;

    /**
     * Returns dims (width, height, depth) of the grid for the given lod
     */
    Vec3i GetGridDims(uint8_t lod = 0) const;

    uint32_t GetCompressedWidth(uint8_t lod = 0) const;

    uint32_t GetCompressedHeight(uint8_t lod = 0) const;

    uint32_t GetCompressedDepth(uint8_t lod = 0) const;

    Vec3i GetCompressedGridDims(uint8_t lod = 0) const;

    /**
     * Returns smallest lod that has dims (1, 1, 1)
     */
    uint8_t GetFirstFullLod() const;

    /**
     * Returns a pointer to data for a given lod
     */
    const uint8_t *GetDataPointer(uint8_t lod) const;

protected:
    /**
     * Convert 3D coordinates to 1D array index (for a given lod)
     */
    int32_t GetArrayIndex(int32_t x, int32_t y, int32_t z, uint8_t lod) const;

    uint8_t GetBitIndex(int32_t x, int32_t y, int32_t z) const;

    bool HasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod) const;

    void SetHasVoxel(int32_t x, int32_t y, int32_t z, uint8_t lod, bool has);

    bool IsCompressed(uint8_t lod) const;

    /**
     * Update all lod-s
     */
    void RecalculateLod();

    /// Dimensions
    uint32_t w = 1, h = 1, d = 1;

    /// Linearized and compressed voxel data (data[0] - original data, data[i] - i-th lod)
    std::vector<uint8_t> data[LOD_NUM];

    /// True if there were any silent updates
    bool silent_updates = false;
};

/**
 * VoxelObject - it is VoxelMesh with size, position and rotation. Also has a unique id.
 */
template<typename T>
class VoxelObject : public VoxelMesh<T>, public Transform {
public:
    VoxelObject(double voxel_size, uint32_t w = 1, uint32_t h = 1, uint32_t d = 1, T fill = 0);

    VoxelObject(double voxel_size, Vec3i dims, T fill = 0);

    ~VoxelObject() override;

    /**
     * Returns size of one voxel for this object
     */
    double GetVoxelSize() const;

    /**
     * Returns unique id of the object
     */
    int GetId() const;

private:
    double voxel_size;
    int id;

    /// Static id counter
    static int nextId;
};

template
class VoxelMesh<uint8_t>;

template
class VoxelMesh<bool>;

template
class VoxelObject<uint8_t>;

template
class VoxelObject<bool>;
