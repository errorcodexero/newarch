#include "WaypointReader.h"
#include "NumUtils.h"
#include "String"
#include <StringUtils.h>
#include <iostream>
#include <fstream>
#include <cctype>

namespace xero
{
	namespace jacispath
	{
		bool WaypointReader::readWaypoint(const std::string &filename, std::vector<Waypoint> &points)
		{
			std::string line;

			std::ifstream in(filename);
			if (in.bad() || in.fail())
			{
				std::cerr << "cannot open file '" << filename << "' for reading" << std::endl;
				return false;
			}

			while (std::getline(in, line))
			{
				std::vector<std::string> words = xero::motion::StringUtils::split(line);
				if (words.size() == 0)
					continue;

				if (words.size() != 3)
				{
					std::cerr << "line: '" << line << "'" << std::endl;
					std::cerr << "      expected three words" << std::endl;
					points.clear();
					return false;
				}

				std::vector<double> values = xero::motion::StringUtils::parseDouble(words);
				if (values.size() == 0)
				{
					points.clear();
					return false;
				}

				double a = values[2] / 180 * PI ;
				a = bound_radians(a);
				Waypoint pt(values[0], values[1], a);
				points.push_back(pt);
			}

			return true;
		}
	}
}
