#include "utils.h"

typedef std::chrono::duration<double, std::ratio<1> > second_;

Timer::Timer() {
    prev_time = std::chrono::high_resolution_clock::now();
}

double Timer::GetElapsedSeconds() {
    auto cur_time = std::chrono::high_resolution_clock::now();
    auto delta = std::chrono::duration_cast<second_>(cur_time - prev_time).count();
    prev_time = cur_time;
    return delta;
}


Random::Random(uint64_t seed) { rng.seed(seed); }

uint64_t Random::get() {
    return rng();
}

int64_t Random::get_int(int64_t max) {
    return get() % (max + 1);
}

int64_t Random::get_int(int64_t min, int64_t max) {
    return get() % (max - min + 1) + min;
}

double Random::get_double() {
    return ((double)get()) / std::numeric_limits<uint64_t>::max();
}

double Random::get_double(double min, double max) {
    return get_double() * (max - min) + min;
}
