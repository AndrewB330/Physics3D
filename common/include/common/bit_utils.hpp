#pragma once

#include <cstdint>

template<typename T>
inline T NextPowerOfTwo(T n) {
    --n;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    if (sizeof(T) > 1) n |= (n >> 8);
    if (sizeof(T) > 2) n |= (n >> 16);
    if (sizeof(T) > 4) n |= (n >> 32);
    return ++n;
}

template<typename T>
inline uint32_t NextPowerOfTwoLog2(T n) {
    uint32_t res = 0;
    n--;
    while(n) {
        n >>= 1;
        ++res;
    }
    return res;
}