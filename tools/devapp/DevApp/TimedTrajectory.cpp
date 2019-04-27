#include "TimedTrajectory.h"

namespace xero
{
	namespace paths
	{
		TimedTrajectory::TimedTrajectory(std::vector<TimedTrajectoryPoint>& points)
		{
			points_ = points;
		}

		TimedTrajectory::~TimedTrajectory()
		{
		}
	}
}
