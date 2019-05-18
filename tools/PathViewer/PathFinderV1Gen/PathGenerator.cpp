#include "PathGenerator.h"

namespace xero
{
	namespace paths
	{

		PathGenerator::PathGenerator(int steps, double timestep)
		{
			steps_ = steps;
			timestep_ = timestep;
		}

		PathGenerator::~PathGenerator()
		{
		}

		TimedTrajectory PathGenerator::generate(const std::vector<Pose2d>& points)
		{
			TimedTrajectory ret;

			return ret;
		}
	}
}
