#include "SimMotorController.h"
#include "RobotSimulator.h"

namespace frc
{
	SimMotorController::SimMotorController(int index)
	{
		m_index = index;
	}

	SimMotorController::~SimMotorController()
	{
	}

	void SimMotorController::Set(double v)
	{
		frc::RobotSimulator &robot = frc::RobotSimulator::get();
		robot.setMotor(m_index, v);
	}
}
