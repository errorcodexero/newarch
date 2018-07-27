#pragma once

#include "SimulatedObject.h"

namespace frc
{
	class Timer : public xero::sim::SimulatedObject
	{
	public:
		Timer();
		virtual ~Timer();

		static double GetFPGATimestamp();

	private:
	};

	void Wait(double secs);
}

