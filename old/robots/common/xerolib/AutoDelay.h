#pragma once

#include "AutonomousStepBase.h"

namespace xero
{
	namespace base
	{
		class AutoDelay : public AutonomousStepBase
		{
		public:
			AutoDelay(AutonomousControllerBase &controller, double duration);
			~AutoDelay();

			virtual void start();
			virtual bool run();
			virtual void end();
			virtual std::string toString();

		private:
			double m_start;
			double m_duration;
		};
	}
}

