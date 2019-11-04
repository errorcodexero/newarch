#include "AutoDelay.h"
#include "XeroRobotBase.h"
#include <sstream>

namespace xero
{
	namespace base
	{
		AutoDelay::AutoDelay(AutonomousControllerBase &controller, double duration) : AutonomousStepBase(controller)
		{
			m_duration = duration;
		}

		AutoDelay::~AutoDelay()
		{
		}

		void AutoDelay::start()
		{
			auto &robot = getRobot();
			m_start = robot.getTime();
		}

		bool AutoDelay::run()
		{
			auto &robot = getRobot();
			double elapsed = robot.getTime() - m_start;
			return elapsed > m_duration;
		}

		void AutoDelay::end()
		{
		}

		std::string AutoDelay::toString()
		{
			std::stringstream strm;

			strm << "delay, duration = " << m_duration << " seconds";
			return strm.str();
		}
	}
}