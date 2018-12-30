#include "GPG3MotorController.h"
#include "GoPiGo3Robot.h"
#include "RobotBase.h"
#include <stdexcept>
#include <iostream>

namespace frc
{
	GPG3MotorController::GPG3MotorController(uint32_t channel)
	{
		inverted_ = false ;
		m_channel = channel;

		if (m_channel == 1)
			m_hw_channel = gopigo3::GoPiGo3Robot::MOTOR_LEFT;
		else if (m_channel == 2)
			m_hw_channel = gopigo3::GoPiGo3Robot::MOTOR_RIGHT;
		else
		{
			std::runtime_error err("illegal GPG3MotorController channel, only 1 (left) and 2 (right) supported");
			throw err;
		}
	}

	GPG3MotorController::~GPG3MotorController()
	{
	}

	void GPG3MotorController::Set(double value, uint8_t syncGroup)
	{
		gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();

		//
		// The GOPIGO3 auto inverts the values to the two sides of the robot.  So this
		// conditional inverts the right side back so that the left and right sides, given the
		// same values, will move the robot in opposite directions.  This is because this is how
		// the robot will likely work in the real world.
		//
		if (m_channel == 2)
			value = -value ;

		//
		// This conditional implements the default SetInverted() method for the FRC motor controllers.
		// If SetInverted(true) is called, then inverted_ will be true.
		//
		if (inverted_)
			value = -value ;

		hw.setMotorPower(m_hw_channel, static_cast<int8_t>(value * 100.0f));
	}

	double GPG3MotorController::Get()
	{
		gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();
		gopigo3::GoPiGo3Robot::MotorStatus st;

		hw.getMotorStatus(m_hw_channel, st);
		return static_cast<double>(st.Power) / 100.0f;
	}

	void GPG3MotorController::Disable()
	{
		gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();
		return hw.setMotorPower(m_hw_channel, 0);
	}

	void GPG3MotorController::StopMotor()
	{
		gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();
		return hw.setMotorPower(m_hw_channel, 0);
	}
}
