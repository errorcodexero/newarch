#include <frc/Timer.h>
#include <chrono>
#include <thread>

namespace frc
{
    std::chrono::time_point<std::chrono::steady_clock> Timer::m_start = std::chrono::steady_clock::now();

    Timer::Timer() {
    }


    Timer::~Timer() {
    }

    void Wait(double time) {
        std::chrono::milliseconds delay((uint32_t)(time * 1000));
        std::this_thread::sleep_for(delay);
    }

    double Timer::GetFPGATimestamp() {
        auto duration = std::chrono::steady_clock::now() - m_start;
        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        return (double)micro / 1.0e6;
    }

    double Timer::getPPCTimestamp() {
        return 0.0;
    }

    double Timer::GetMatchTime() {
        return 0.0;
    }
}

