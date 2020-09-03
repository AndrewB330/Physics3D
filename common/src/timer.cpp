#include <common/timer.hpp>

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
