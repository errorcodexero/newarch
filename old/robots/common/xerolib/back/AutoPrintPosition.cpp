#include "AutoPrintPosition.h"
#include "RobotState.h"
#include "MessageLogger.h"
#include "ModuleDefintions.h"

using namespace xero::pathfinder;

namespace xerolib
{
	AutoPrintPosition::AutoPrintPosition(AutonomousControllerBase &controller) 
		: AutonomousStepBase(controller)
	{
	}

	AutoPrintPosition::~AutoPrintPosition()
	{
	}

	void AutoPrintPosition::start()
	{
	}

	bool AutoPrintPosition::run()
	{
		RobotState &st = RobotState::get();
		MessageLogger &logger = getRobot().getMessageLogger();

		const auto &data = st.getPositions();
		int sample = 0;
		for (const auto &pos : data)
		{
			logger.startMessage(MessageLogger::MessageType::Debug, MODULE_AUTOMODE);
			logger << "Sample " << sample++;
			logger << ": Time " << pos.first;
			logger << ", Positions " << pos.second.toString();
			logger.endMessage();
		}

		return true;
	}

	void AutoPrintPosition::end()
	{
	}

	std::string AutoPrintPosition::toString()
	{
		return "AutoPrintPosition";
	}
}
