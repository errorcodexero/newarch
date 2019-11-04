#include "IntelligentDriveBase.h"
#include "DriveBase.h"
#include "OdometrySystem.h"
#include "PathFollowerController.h"
#include "XeroRobotBase.h"

namespace xero
{
	namespace base
	{
		IntelligentDriveBase::IntelligentDriveBase(XeroRobotBase &robot, std::shared_ptr<DriveBase> db_p,
			std::shared_ptr<OdometrySystem> od_p, std::shared_ptr<PathFollowerController> pf_p) : SubsystemBase("IntelligentDriveBase", robot)
		{
			m_db_p = db_p;
			m_odometry_p = od_p;
			m_path_follower_p = pf_p;

			m_distances.resize(m_db_p->getNumberWheels());
		}

		IntelligentDriveBase::~IntelligentDriveBase()
		{
		}

		void IntelligentDriveBase::getInputs()
		{
			double t = getRobot().getTime();
			m_db_p->getInputs();

			m_odometry_p->update(t, m_db_p->getAngle().getRadians(), m_db_p->getDistances());
		}

		void IntelligentDriveBase::setOutputs()
		{
		}
	}
}
