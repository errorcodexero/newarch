#pragma once

#include <cstdint>

namespace frc
{
	class Servo
	{
	public:
		Servo(uint8_t index);
		~Servo();

		void setPosition(float angle);

	private:
		static const float PULSE_WIDTH_RANGE ;

	private:
		uint8_t m_hw_channel;
	};
}
