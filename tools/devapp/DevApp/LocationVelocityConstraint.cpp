#include "LocationVelocityConstraint.h"


namespace xero
{
	namespace paths
	{
		LocationVelocityConstraint::LocationVelocityConstraint(const Translation2d& loc, double distance, double velocity)
		{
			loc_ = loc;
			distance_ = distance;
			velocity_ = velocity;
		}

		LocationVelocityConstraint::~LocationVelocityConstraint()
		{
		}


		double LocationVelocityConstraint::getMaxVelocity(const ConstrainedPoint& state)
		{
			if (state.getState()->getTranslation().distance(loc_) < distance_)
				return velocity_;

			return std::numeric_limits<double>::max();
		}

		MinMaxAcceleration LocationVelocityConstraint::getMinMacAccel(const ConstrainedPoint& state, double velocity)
		{
			return MinMaxAcceleration();
		}
	}
}
