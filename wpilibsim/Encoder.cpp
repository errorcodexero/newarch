#include "Encoder.h"
#include "RobotSimulator.h"

namespace frc
{
	Encoder::Encoder(int first, int second)
	{
		m_first = first;
		m_second = second;
	}

	Encoder::~Encoder()
	{
	}

	void Encoder::Reset()
	{
		frc::RobotSimulator &robot = frc::RobotSimulator::get();
		robot.resetEncoder(m_first, m_second);
	}

	int32_t Encoder::Get()
	{
		frc::RobotSimulator &robot = frc::RobotSimulator::get();
		return robot.getEncoder(m_first, m_second);
	}
}
