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
		static constexpr float PULSE_WIDTH_RANGE = 1850.0f ;

	private:
		uint8_t m_hw_channel;
	};
}
