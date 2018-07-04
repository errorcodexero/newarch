#include "Timer.h"
#include <chrono>

namespace frc
{
	std::chrono::steady_clock::time_point Timer::m_start = std::chrono::steady_clock::now();

	Timer::Timer()
	{
	}

	Timer::~Timer()
	{
	}

	double Timer::GetFPGATimestamp()
	{
		auto duration = std::chrono::steady_clock::now() - m_start;
		auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
		return (double)count / 1.0e6;
	}

	void Wait(double secs)
	{
		long long micro = static_cast<long long>(secs * 1e6);
		std::chrono::microseconds delay(micro);
	}
}
