#pragma once

namespace xero 
{
	namespace paths
	{
		class MathUtils
		{
		public:
			MathUtils() = delete;
			~MathUtils() = delete;

			static double const kEpsilon;
			static double const kPI;

			static bool epsilonEqual(double a, double b, double ep = kEpsilon);
			static double interpolate(double first, double second, double percent);
			static double radiansToDegrees(double rads);
			static double degreesToRadians(double degrees);
		};
	}
}

