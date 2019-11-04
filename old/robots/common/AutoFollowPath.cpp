#include "AutoFollowPath.h"
#include "AutonomousControllerBase.h"
#include "TankDrive.h"
#include "ModuleDefintions.h"
#include <sstream>

namespace xero
{
	namespace base
	{
		AutoFollowPath::AutoFollowPath(AutonomousControllerBase &controller, std::shared_ptr<TankDrive> drive_p,
			const std::string &pathname, const std::string &filename) : AutonomousStepBase(controller)
		{
			m_db_p = drive_p;

			if (!m_path.create(pathname, filename, drive_p->getWidth()))
			{
				MessageLogger &logger = controller.getRobot().getMessageLogger();

				logger.startMessage(MessageLogger::MessageType::Error, MODULE_AUTOMODE);
				logger << "cannot load path from file '" << filename << "' - " << m_path.getErrorMessage();
				logger.endMessage();
			}
		}

		AutoFollowPath::~AutoFollowPath()
		{
		}

		void AutoFollowPath::start()
		{
		}

		bool AutoFollowPath::run()
		{
		}

		void AutoFollowPath::end()
		{
		}

		std::string AutoFollowPath::toString()
		{
			std::stringstream strm;

			strm << "follow path, path='" << m_path.getName() << "'";
			return strm.str();
		}
	}
}