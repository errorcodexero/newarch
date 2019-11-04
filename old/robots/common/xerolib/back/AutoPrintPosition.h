#pragma once

#include "AutonomousStepBase.h"
#include "DriveBase.h"
#include <memory>

namespace xerolib
{
	class AutoPrintPosition : public AutonomousStepBase
	{
	public:
		AutoPrintPosition(AutonomousControllerBase &controller);
		virtual ~AutoPrintPosition();

		virtual void start();
		virtual bool run();
		virtual void end();
		virtual std::string toString();

	private:
	};
}
