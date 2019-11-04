#pragma once

#include "OdometrySystem.h"

namespace xero
{
	namespace base
	{
		class KalmanOdometry : public OdometrySystem
		{
		public:
			KalmanOdometry();
			virtual ~KalmanOdometry();

			/// @brief called to update the location of the robot
			/// @param t current time
			/// @param gyro the angle of the robot according to the gyro
			/// @param distances the distance each wheel on the robot has traveled
			virtual void update(double t, double gyro, std::vector<double> &distances);
		};
	}
}
