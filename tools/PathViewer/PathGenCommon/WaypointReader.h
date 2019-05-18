#pragma once
#include "Pose2d.h"
#include <vector>
#include <string>


namespace xero
{
	namespace paths
	{
		class WaypointReader
		{
		public:
			WaypointReader() = delete;;
			virtual ~WaypointReader() = delete;

			static bool readWaypoints(const std::string& filename, std::vector<Pose2d>& points);

		private:
			static std::vector<std::string> split(const std::string& str, char ch);
			static bool getNumbers(const std::vector<std::string>& strings, std::vector<double>& numbers);
		};
	}
}

