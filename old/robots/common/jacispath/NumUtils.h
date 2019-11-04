#pragma once
#include <algorithm>

namespace xero
{
	namespace jacispath
	{
		static double constexpr PI = 3.14159265358979323846;

		double inline clamp(double value, double min, double max)
		{
			return std::min(std::max(min, value), max);
		}

		double inline bound_radians(double rad)
		{
			double ang = std::fmod(rad, PI * 2.0);
			if (ang < 0.0)
				ang += PI * 2.0 ;

			return ang;
		}

		double inline d2r(double deg)
		{
			return deg / 180.0 * PI;
		}

		double inline r2d(double rad)
		{
			return rad / PI * 180.0;
		}
	}
}
