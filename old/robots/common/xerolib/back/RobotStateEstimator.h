#pragma once

#include "DriveBase.h"
#include <string>
#include <fstream>

namespace xero
{
	namespace pathfinder
	{
		class RobotStateEstimator
		{
		public:
			RobotStateEstimator(const std::string &filename);
			~RobotStateEstimator();

			static RobotStateEstimator &get()
			{
				if (TheOneObject == nullptr)
					TheOneObject = new RobotStateEstimator(LogFileName);

				return *TheOneObject;
			}

			void setDriveBase(xerolib::DriveBase *db_p)
			{
				m_db_p = db_p;
			}

			void oneLoop(double t);

			static std::string LogFileName;

		private:
			static RobotStateEstimator *TheOneObject;

			double m_left_dist_prev;
			double m_right_dist_prev;

			xerolib::DriveBase *m_db_p;

			std::ofstream m_logfile;
		};
	}
}

