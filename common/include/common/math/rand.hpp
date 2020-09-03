#pragma once
#include <random>
#include <chrono>

class RandomGen {
private:
    std::mt19937_64 rng;
public:
    explicit RandomGen(uint64_t seed);

    uint64_t get();

    int64_t get_int(int64_t max);

    int64_t get_int(int64_t min, int64_t max);

    double get_double();

    double get_double(double min, double max);

    template<typename T>
    T choose(const std::vector<T>& vec) {
        return vec[get_int(vec.size() - 1)];
    }
};
