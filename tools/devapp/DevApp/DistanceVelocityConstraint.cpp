#include "DistanceVelocityConstraint.h"

namespace xero {
	namespace paths {
		DistanceVelocityConstraint::DistanceVelocityConstraint(double dist, double velocity)
		{
			before_distance_ = dist;
			velocity_ = velocity;
		}

		DistanceVelocityConstraint::~DistanceVelocityConstraint()
		{
		}

		double DistanceVelocityConstraint::getMaxVelocity(const ConstrainedPoint& state)
		{
			if (state.getDistance() < before_distance_)
				return velocity_;

			return std::numeric_limits<double>::max();
		}

		MinMaxAcceleration DistanceVelocityConstraint::getMinMacAccel(const ConstrainedPoint& state, double velocity)
		{
			return MinMaxAcceleration();
		}
	}
}
