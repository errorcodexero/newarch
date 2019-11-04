#include "RobotControllerBase.h"

namespace xero
{
	namespace base
	{
		RobotControllerBase::RobotControllerBase(XeroRobotBase &robot) : m_robot(robot)
		{
		}

		RobotControllerBase::~RobotControllerBase()
		{
		}
	}
}
