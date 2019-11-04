#include "StringUtils.h"
#include <cctype>
#include <iostream>

namespace xero
{
	namespace motion
	{
		std::vector<std::string> StringUtils::split(const std::string &line)
		{
			std::vector<std::string> ret;
			std::string word;

			size_t i = 0;
			while (i < line.length())
			{
				//
				// Skip leading spaces
				//
				while (i < line.length() && std::isspace(line[i]))
					i++;

				if (i == line.length())
					break;

				//
				// Capture the next wore
				//
				word.clear();
				while (i < line.length() && !std::isspace(line[i]))
					word += line[i++];

				//
				// Push the word onto the result
				//
				ret.push_back(word);
			}

			return ret;
		}

		std::vector<std::string> StringUtils::split(const std::string &line, char ch)
		{
			std::vector<std::string> ret;
			std::string word;

			size_t i = 0;
			while (i < line.length())
			{
				//
				// Capture the next wore
				//
				word.clear();
				while (i < line.length() && line[i] != ch)
					word += line[i++];

				//
				// SKip the delimeter character
				//
				i++;

				//
				// Push the word onto the result
				//
				ret.push_back(word);
			}

			return ret;
		}

		std::vector<double> StringUtils::parseDouble(const std::vector<std::string> &words)
		{
			std::vector<double> values;
			for (size_t i = 0; i < words.size(); i++)
			{
				size_t end;
				double x = std::stod(words[i], &end);
				if (end != words[i].length())
				{
					std::cerr << "      expected all floating point values" << std::endl;
					std::cerr << "      word '" << words[i] << "' is not a valid floating point number" << std::endl;
					values.clear();
				}

				values.push_back(x);
			}

			return values;
		}
	}
}
