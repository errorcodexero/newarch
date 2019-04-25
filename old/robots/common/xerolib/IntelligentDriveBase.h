#pragma once

#include "SubsystemBase.h"
#include <vector>

namespace xero
{
	namespace base
	{
		class PathFollowerController;
		class OdometrySystem;
		class DriveBase;

		class IntelligentDriveBase : public SubsystemBase
		{
		public:
			IntelligentDriveBase(XeroRobotBase &robot, std::shared_ptr<DriveBase> db_p, 
				std::shared_ptr<OdometrySystem> od_p, std::shared_ptr<PathFollowerController> pf_p);
			virtual ~IntelligentDriveBase();

			virtual void getInputs() ;
			virtual void setOutputs() ;

		private:
			std::shared_ptr<DriveBase> m_db_p;
			std::shared_ptr<OdometrySystem> m_odometry_p;
			std::shared_ptr<PathFollowerController> m_path_follower_p;

			double m_last_distance_x;
			double m_last_distance_y;

			std::vector<double> m_distances;
		};
	}
}

