#pragma once

#include "TimedTrajectory.h"
#include "Trajectory.h"
#include "DistanceView.h"
#include "TimingConstraint.h"
#include <list>
#include <memory>

namespace xero {
	namespace paths
	{

		class TimingUtil
		{
		public:
			TimingUtil() = delete;
			~TimingUtil() = delete ;

			static TimedTrajectory timeParameterizeTrajectory(DistanceView& distview, const ConstraintList &constraints, double step_size,
									double startvel, double endvel, double maxvel, double maxaccel);

			static TimedTrajectory timeParameterizeTrajectory(std::vector<TrajectorySamplePoint> &states, const ConstraintList& constraints, 
									double startvel, double endvel, double maxvel, double maxaccel);

		private:
			static double constexpr kEpsilon = 1e-6;
		};
	}
}

