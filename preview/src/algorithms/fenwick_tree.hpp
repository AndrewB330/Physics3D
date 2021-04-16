#pragma once

#include <math/vec3i.hpp>
#include <vector>

class FenwickTreeRange3D {
public:
    FenwickTreeRange3D(Vec3i dims);

    void Reset(Vec3i dims);

    void AddRange(Vec3i begin, Vec3i end, int64_t value);

    void Add(Vec3i pos, int64_t value);

    int64_t GetRange(Vec3i begin, Vec3i end);

    int64_t GetRange(Vec3i end);
private:
    int PosToIndex(int i, int j, int k) const;

    std::vector<int64_t> data;
    int w, h, d;
};