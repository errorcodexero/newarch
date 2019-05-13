#include "TimedTrajectoryPoint.h"

namespace xero
{
	namespace paths
	{
		TimedTrajectoryPoint::TimedTrajectoryPoint(const TrajectorySamplePoint& pt, double t, double p, double v, double a) : TrajectorySamplePoint(pt)
		{
			time_ = t;
			position_ = p;
			velocity_ = v;
			acceleration_ = a;
		}

		TimedTrajectoryPoint::~TimedTrajectoryPoint()
		{
		}
	}
}
