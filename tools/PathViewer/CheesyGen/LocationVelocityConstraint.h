#pragma once

#include "TimingConstraint.h"
#include "Translation2d.h"

namespace xero
{
	namespace paths
	{
		class LocationVelocityConstraint : public TimingConstraint
		{
		public:
			LocationVelocityConstraint(const Translation2d &loc, double distance, double velocity);
			virtual ~LocationVelocityConstraint();

			virtual double getMaxVelocity(const ConstrainedPoint& state);
			virtual MinMaxAcceleration getMinMacAccel(const ConstrainedPoint& state, double velocity);

		private:
			Translation2d loc_;
			double distance_;
			double velocity_;
		};
	}
}

