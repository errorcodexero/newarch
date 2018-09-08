#pragma once

#include "PWMSpeedController.h"
#include <cstdint>

namespace frc
{
	class GPG3MotorController : public PWMSpeedController
	{
	public:
		GPG3MotorController(uint32_t channel);
		virtual ~GPG3MotorController();

		virtual void Set(double value, uint8_t syncGroup = 0);
		virtual double Get();
		virtual void Disable();
		virtual void StopMotor();

	private:
		uint32_t m_channel;
		uint8_t m_hw_channel;
	};
}

