#include "AutoSetVelocity.h"
#include <sstream>

namespace xero
{
	namespace base
	{
		AutoSetVelocity::AutoSetVelocity(AutonomousControllerBase &ctrl, std::shared_ptr<TankDrive> drive_p, double duration, double left, double right)
			: AutonomousStepBase(ctrl)
		{
			m_db_p = drive_p;
			m_duration = duration;
			m_left = left;
			m_right = right;
		}

		AutoSetVelocity::~AutoSetVelocity()
		{
		}


		void AutoSetVelocity::start()
		{
			XeroRobotBase &robot = getRobot();
			m_start = robot.getTime();
			m_db_p->setVelocity(m_left, m_right);
		}

		bool AutoSetVelocity::run()
		{
			bool ret = false;
			XeroRobotBase &robot = getRobot();
			double elapsed = robot.getTime() - m_start;

			if (elapsed > m_duration)
			{
				m_db_p->stop();
				ret = true;
			}
			return ret;
		}

		void AutoSetVelocity::end()
		{
		}

		std::string AutoSetVelocity::toString()
		{
			std::stringstream strm;

			strm << "set motor velocity, duration = " << m_duration << " seconds, ";
			strm << "left = " << m_left << ", right = " << m_right;

			return strm.str();
		}
	}
}