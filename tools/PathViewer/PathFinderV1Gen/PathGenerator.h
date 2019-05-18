#pragma once

#include "TimedTrajectory.h"

namespace xero
{
	namespace paths
	{
		class PathGenerator
		{
		public:
			PathGenerator(int steps, double timestep);
			~PathGenerator();

			TimedTrajectory generate(const std::vector<Pose2d>& points);

		private:
			int steps_;
			double timestep_;
		};
	}
}

