#pragma once

#include "TimingConstraint.h"

namespace xero {
	namespace paths {

		class DistanceVelocityConstraint : public TimingConstraint
		{
		public:
			DistanceVelocityConstraint(double after, double before, double velocity);
			virtual ~DistanceVelocityConstraint();

			virtual double getMaxVelocity(const ConstrainedPoint& state) ;
			virtual MinMaxAcceleration getMinMacAccel(const ConstrainedPoint& state, double velocity) ;

		private:
			double after_distance_;
			double before_distance_;
			double velocity_;
		};
	}
}

