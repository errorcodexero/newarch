#pragma once

#include "SubsystemBase.h"
#include "Rotation.h"
#include <vector>

namespace xero
{
	namespace base
	{
		class DriveBase : public SubsystemBase
		{
		public:
			DriveBase(const char *name_p, XeroRobotBase &robot);
			~DriveBase();

			virtual xero::base::Rotation getAngle() = 0;
			virtual double getLeftDistance() = 0;
			virtual double getRightDistance() = 0;
			virtual size_t getNumberWheels() = 0;
			virtual std::vector<double> &getDistances() = 0;
		};
	}
}

