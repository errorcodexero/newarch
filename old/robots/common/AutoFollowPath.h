#pragma once

#include "AutonomousStepBase.h"
#include "Path.h"

namespace xero
{
	namespace base
	{
		class IntelligentDriveBase ;

		class AutoFollowPath : public AutonomousStepBase
		{
		public:
			AutoFollowPath(AutonomousControllerBase &controller, std::shared_ptr<TankDrive> drive_p,
				const std::string &pathname, const std::string &filename);
			~AutoFollowPath();

			virtual void start();
			virtual bool run();
			virtual void end();
			virtual std::string toString();

		private:
			xero::motion::Path m_path;
			std::shared_ptr<IntelligentDriveBase> m_db_p ;
		};
	}
}