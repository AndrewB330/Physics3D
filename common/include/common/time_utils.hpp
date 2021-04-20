#pragma once
#include <chrono>
#include <queue>

class Timer {
public:
    Timer();

    void Step();

    double GetElapsedSinceStep();

    double GetElapsedSinceStart();

private:
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> prev_time;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start_time;
};

class FPSMeter {
public:
    FPSMeter() = default;

    void StartFrame();

    void EndFrame();

    double GetFPS() const;

    double GetMs() const;

private:
    std::queue<double> times;

    const int MAX_FRAMES_COUNT = 120;

    Timer timer;
};
