#include "MathUtils.h"
#include <cmath>

namespace xero {
	namespace paths
	{
		const double MathUtils::kEpsilon = 1e-5;
		const double MathUtils::kPI = 3.14159265358979323846264338327950288419716939937510582097494459230;

		bool MathUtils::epsilonEqual(double a, double b, double eps)
		{
			return std::fabs(a - b) < eps;
		}

		double MathUtils::interpolate(double a, double b, double percent)
		{
			if (percent < 0)
				percent = 0;
			else if (percent > 1)
				percent = 1;
			return a + (b - a) * percent;
		}

		double MathUtils::radiansToDegrees(double rads)
		{
			return rads / kPI * 180.0;
		}

		double MathUtils::degreesToRadians(double degrees)
		{
			return degrees / 180.0 * kPI;
		}
	}
}
