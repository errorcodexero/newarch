#pragma once

#include "TrajectoryPoint.h"

namespace xero {
	namespace paths
	{
		class TrajectorySamplePoint : public Pose2dWithCurvature
		{
		public:
			TrajectorySamplePoint();
			TrajectorySamplePoint(const TrajectoryPoint& p);
			TrajectorySamplePoint(const Pose2dWithCurvature& p, size_t minindex, size_t maxindex);
			virtual ~TrajectorySamplePoint();

			size_t index_floor() const {
				return index_floor_;
			}

			size_t index_ceil() {
				return index_ceil_;
			}

		private:
			size_t index_floor_;
			size_t index_ceil_;
		};
	}
}

