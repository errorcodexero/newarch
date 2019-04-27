#pragma once

#include "TimedTrajectory.h"
#include "Trajectory.h"
#include "DistanceView.h"

namespace xero {
	namespace paths
	{
		class TimingUtil
		{
		public:
			TimingUtil() = delete;
			~TimingUtil() = delete ;

			static TimedTrajectory timeParameterizeTrajectory(bool reverse, DistanceView& distview, double step_size, double startvel, double endvel, double maxvel, double maxaccel);
			static TimedTrajectory timeParameterizeTrajectory(bool reverse, std::vector<TrajectorySamplePoint> &states, double startvel, double endvel, double maxvel, double maxaccel);

		private:
			static double constexpr kEpsilon = 1e-6;
		};
	}
}

