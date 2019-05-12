#pragma once

#include "TimedTrajectoryPoint.h"
#include <vector>

namespace xero
{
	namespace paths
	{
		class TimedTrajectory
		{
		public:
			TimedTrajectory();
			TimedTrajectory(std::vector<TimedTrajectoryPoint>& points);
			virtual ~TimedTrajectory();

			const std::vector<TimedTrajectoryPoint> &getPoints() {
				return points_;
			}

		private:
			std::vector<TimedTrajectoryPoint> points_;
		};
	}
}

