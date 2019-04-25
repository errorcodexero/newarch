#pragma once
#include "AutonomousStepBase.h"
#include "TankDrive.h"

namespace xero
{
	namespace base
	{
		class AutoSetVelocity : public AutonomousStepBase
		{
		public:
			AutoSetVelocity(AutonomousControllerBase &ctrl, std::shared_ptr<TankDrive> drive_p, double duration, double left, double right);
			~AutoSetVelocity();

			virtual void start();
			virtual bool run();
			virtual void end();
			virtual std::string toString();

		private:
			std::shared_ptr<TankDrive> m_db_p;
			double m_start;
			double m_duration;
			double m_left;
			double m_right;
		};
	}
}

