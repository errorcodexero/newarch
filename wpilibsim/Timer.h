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

	private:
		static std::chrono::steady_clock::time_point m_start;
	};

	void Wait(double secs);
}

