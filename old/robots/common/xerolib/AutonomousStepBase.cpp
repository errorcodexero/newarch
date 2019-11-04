#include "AutonomousStepBase.h"
#include "AutonomousControllerBase.h"
#include "XeroRobotBase.h"

namespace xero
{
	namespace base
	{
		AutonomousStepBase::AutonomousStepBase(AutonomousControllerBase &controller) : m_controller(controller)
		{
		}

		AutonomousStepBase::~AutonomousStepBase()
		{
		}

		XeroRobotBase &AutonomousStepBase::getRobot()
		{
			return m_controller.getRobot();
		}

		MessageLogger &AutonomousStepBase::getLogger()
		{
			return getRobot().getMessageLogger();
		}
	}
}