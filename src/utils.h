#pragma once
#include <random>
#include <chrono>

class Timer {
private:
    std::chrono::steady_clock::time_point prev_time;
public:
    Timer();

    double GetElapsedSeconds();
};

class Random {
private:
    std::mt19937_64 rng;
public:
    explicit Random(uint64_t seed);

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