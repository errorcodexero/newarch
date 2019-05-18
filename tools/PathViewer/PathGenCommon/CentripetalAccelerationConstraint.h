#pragma once

#include "TimingConstraint.h"
#include "ConstrainedPoint.h"
#include <cmath>

namespace xero
{
	namespace paths
	{
		class CentripetalAccelerationConstraint : public TimingConstraint
		{
		public:
			CentripetalAccelerationConstraint(double accel) {
				maxCentripetalAccel_ = accel;
			}

			virtual ~CentripetalAccelerationConstraint() {
			}

			virtual double getMaxVelocity(const ConstrainedPoint& st) {
				return std::sqrt(maxCentripetalAccel_ / st.getState()->getCurvature());
			}

			virtual MinMaxAcceleration getMinMacAccel(const ConstrainedPoint& st, double velocity) {
				return MinMaxAcceleration();
			}

		private:
			double maxCentripetalAccel_;
		};
	}
}

