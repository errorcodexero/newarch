#include "TimedTrajectoryPoint.h"

namespace xero
{
	namespace paths
	{
		TimedTrajectoryPoint::TimedTrajectoryPoint(const TrajectorySamplePoint& pt, double t, double v, double a) : TrajectorySamplePoint(pt)
		{
			time_ = t;
			velocity_ = v;
			acceleration_ = a;
		}

		TimedTrajectoryPoint::~TimedTrajectoryPoint()
		{
		}
	}
}
