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
	}
}
