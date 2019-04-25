#pragma once

#include "AutonomousStepBase.h"
#include <memory>

namespace xerolib
{
	class DriveBase;

	class AutoSetMotorVoltage : public AutonomousStepBase
	{
	public:
		AutoSetMotorVoltage(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> drive_p, double duration, double left, double right);
		~AutoSetMotorVoltage();

		virtual void start() ;
		virtual bool run() ;
		virtual void end() ;
		virtual std::string toString() ;

	private:
		std::shared_ptr<DriveBase> m_drivebase_p;
		double m_duration;
		double m_start;
		double m_left_voltage;
		double m_right_voltage;
	};
}
