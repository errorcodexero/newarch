#pragma once

#include <cstdint>

namespace frc
{
	class SpeedController
	{
	public:
		SpeedController()
		{
		}

		virtual ~SpeedController()
		{
		}

		virtual void Set(double value, uint8_t syncgroup = 0) = 0;
		virtual double Get() = 0;
		virtual void Disable() = 0;
		virtual void StopMotor() = 0;
	};
}
