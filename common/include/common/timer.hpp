#pragma once
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> prev_time;
public:
    Timer();

    double GetElapsedSeconds();
};
