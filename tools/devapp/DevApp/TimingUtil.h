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
		typedef std::list<std::shared_ptr<TimingConstraint>> ConstraintList;
		class TimingUtil
		{
		public:
			TimingUtil() = delete;
			~TimingUtil() = delete ;

			static TimedTrajectory timeParameterizeTrajectory(bool reverse, DistanceView& distview, ConstraintList &constraints, double step_size,
									double startvel, double endvel, double maxvel, double maxaccel);

			static TimedTrajectory timeParameterizeTrajectory(bool reverse, std::vector<TrajectorySamplePoint> &states, ConstraintList& constraints, 
									double startvel, double endvel, double maxvel, double maxaccel);

		private:
			static double constexpr kEpsilon = 1e-6;
		};
	}
}

