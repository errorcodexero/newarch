#pragma once

#include <vector>
#include <string>

namespace xero
{
	namespace motion
	{
		class StringUtils
		{
		public:
			StringUtils() = delete;
			~StringUtils() = delete;

			static std::vector<std::string> split(const std::string &line);
			static std::vector<std::string> split(const std::string &line, char ch);
			static std::vector<double> parseDouble(const std::vector<std::string> &words);

		};
	}
}

