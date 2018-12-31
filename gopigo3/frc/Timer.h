#pragma once

#include <chrono>

namespace frc
{
    class Timer
    {
    public:
        Timer();
        virtual ~Timer();

        static double GetFPGATimestamp();
        static double getPPCTimestamp();
        static double GetMatchTime();

        void Reset() {
        }

        void Start() {
        }

        double Get() {
            return 0.0;
        }

        static std::chrono::time_point<std::chrono::steady_clock> m_start;
    };

    void Wait(double);
}
