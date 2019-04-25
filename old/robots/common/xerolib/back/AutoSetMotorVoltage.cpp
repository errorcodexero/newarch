#include "AutoSetMotorVoltage.h"
#include "DriveBase.h"
#include "XeroRobotBase.h"
#include <sstream>

namespace xerolib
{
	AutoSetMotorVoltage::AutoSetMotorVoltage(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> drive_p, 
		double duration, double left, double right) 	: AutonomousStepBase(controller)
	{
		m_drivebase_p = drive_p;
		m_duration = duration;
		m_left_voltage = left;
		m_right_voltage = right;
	}

	AutoSetMotorVoltage::~AutoSetMotorVoltage()
	{
	}

	void AutoSetMotorVoltage::start()
	{
		XeroRobotBase &robot = getRobot();
		m_start = robot.getTime();
		m_drivebase_p->setMotorVoltage(m_left_voltage, m_right_voltage);
	}

	bool AutoSetMotorVoltage::run()
	{
		bool ret = false;
		XeroRobotBase &robot = getRobot();
		double elapsed = robot.getTime() - m_start;

		if (elapsed > m_duration)
		{
			m_drivebase_p->stop();
			ret = true;
		}
		return ret;
	}

	void AutoSetMotorVoltage::end()
	{
	}

	std::string AutoSetMotorVoltage::toString()
	{
		std::stringstream strm;

		strm << "set motor voltage, duration = " << m_duration << " seconds, ";
		strm << "left voltage = " << m_left_voltage << ", right voltage = " << m_right_voltage;

		return strm.str();
	}
}