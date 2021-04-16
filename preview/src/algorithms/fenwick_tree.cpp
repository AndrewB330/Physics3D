#include "fenwick_tree.hpp"

FenwickTreeRange3D::FenwickTreeRange3D(Vec3i dims) {
    Reset(dims);
}

void FenwickTreeRange3D::Reset(Vec3i dims) {
    w = dims.x;
    h = dims.y;
    d = dims.z;
    data.assign(w * h * d, 0);
}

int64_t FenwickTreeRange3D::GetRange(Vec3i end) {
    int64_t res = 0;
    for (int i = end.x - 1; i >= 0; i = (i & (i + 1)) - 1)
        for (int j = end.y - 1; j >= 0; j = (j & (j + 1)) - 1)
            for (int k = end.z - 1; k >= 0; k = (k & (k + 1)) - 1)
                res += data.at(PosToIndex(i, j, k));
    return res;
}

int64_t FenwickTreeRange3D::GetRange(Vec3i begin, Vec3i end) {
    return GetRange(end)
           - GetRange(Vec3i(begin.x, end.y, end.z))
           - GetRange(Vec3i(end.x, begin.y, end.z))
           - GetRange(Vec3i(end.x, end.y, begin.z))
           + GetRange(Vec3i(end.x, begin.y, begin.z))
           + GetRange(Vec3i(begin.x, end.y, begin.z))
           + GetRange(Vec3i(begin.x, begin.y, end.z))
           - GetRange(Vec3i(begin.x, begin.y, begin.z));
}

void FenwickTreeRange3D::AddRange(Vec3i begin, Vec3i end, int64_t value) {
    for (int64_t i = begin.x; i < end.x; i++)
        for (int64_t j = begin.y; j < end.y; j++)
            for (int64_t k = begin.z; k < end.z; k++)
                Add(Vec3i(i, j, k), value);
}

void FenwickTreeRange3D::Add(Vec3i pos, int64_t value) {
    for (int64_t i = pos.x; i < w; i = (i | (i + 1)))
        for (int64_t j = pos.y; j < h; j = (j | (j + 1)))
            for (int64_t k = pos.z; k < d; k = (k | (k + 1)))
                data.at(PosToIndex(i, j, k)) += value;
}

int FenwickTreeRange3D::PosToIndex(int i, int j, int k) const {
    return i + j * w + k * w * h;
}
