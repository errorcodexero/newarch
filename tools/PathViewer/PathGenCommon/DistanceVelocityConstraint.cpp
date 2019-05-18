#include "DistanceVelocityConstraint.h"

namespace xero {
	namespace paths {
		DistanceVelocityConstraint::DistanceVelocityConstraint(double after, double before, double velocity)
		{
			after_distance_ = after;
			before_distance_ = before;
			velocity_ = velocity;
		}

		DistanceVelocityConstraint::~DistanceVelocityConstraint()
		{
		}

		double DistanceVelocityConstraint::getMaxVelocity(const ConstrainedPoint& state)
		{
			if (state.getDistance() >= after_distance_ && state.getDistance() < before_distance_)
				return velocity_;

			return std::numeric_limits<double>::max();
		}

		MinMaxAcceleration DistanceVelocityConstraint::getMinMacAccel(const ConstrainedPoint& state, double velocity)
		{
			return MinMaxAcceleration();
		}
	}
}
