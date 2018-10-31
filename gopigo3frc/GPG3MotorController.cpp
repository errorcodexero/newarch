#include "GPG3MotorController.h"
#include "GoPiGo3Robot.h"
#include "RobotBase.h"
#include <stdexcept>

namespace frc
{
	GPG3MotorController::GPG3MotorController(uint32_t channel)
	{
		m_channel = channel;

		if (m_channel == 0)
			m_hw_channel = gopigo3::GoPiGo3Robot::MOTOR_LEFT;
		else if (m_channel == 1)
			m_hw_channel = gopigo3::GoPiGo3Robot::MOTOR_RIGHT;
		else
		{
			std::runtime_error err("illegal GPG3MotorController channel, only 0 (left) and 1 (right) supported");
			throw err;
		}
	}

	GPG3MotorController::~GPG3MotorController()
	{
	}

	void GPG3MotorController::Set(double value, uint8_t syncGroup)
	{
		gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();

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
