#include "SubsystemBase.h"

namespace xerolib
{
	SubsystemBase::SubsystemBase(const char *name_p, XeroRobotBase &robot) : m_robot(robot)
	{
		m_name = name_p;
	}

	SubsystemBase::~SubsystemBase()
	{
	}
}
