#include "ParamsParser.h"
#include <ostream>
#include <string>
#include <iostream>

namespace xero
{
	namespace base
	{
		ParamsParser ParamsParser::mSingleObject;

		ParamsParser::ParamsParser()
		{
		}

		bool ParamsParser::readFile(const std::string &filename)
		{
			bool ret = false;
			this->mFilename = filename;

			std::ifstream f(filename);
			if (f.is_open())
			{
				std::string buffer;

				while (!f.eof())
				{
					std::getline(f, buffer);
					unsigned int commentStart = buffer.find('#');

					if (commentStart != buffer.npos)
					{
						buffer = buffer.substr(0, commentStart);
					}

					if (buffer.length() > 0)
					{
						unsigned int i = 0;
						std::string Key, valueString;
						double value;
						while (i < buffer.length() && (!isspace(buffer[i])))
							Key += buffer[i++];

						while (i < buffer.length() && (isspace(buffer[i])))
							i++;

						while (i < buffer.length() && (!isspace(buffer[i])))
							valueString += buffer[i++];

						try
						{
							value = std::stod(valueString);
						}
						catch (...)
						{
							std::cout << "cannot parse string '" << valueString << "' as a decimal" << std::endl;
							std::cout << "key is '" << Key << "'" << std::endl;
						}
						mParamsMap.insert(std::pair<std::string, double>(Key, value));
					}
				}
				ret = true;
			}
			else
			{
				std::cout << "Could not open file '" << filename << "'" << std::endl;
			}

			return ret;
		}

		bool ParamsParser::printMap(std::ostream &fo)
		{
			for (auto it = mParamsMap.begin(); it != mParamsMap.end(); it++)
			{
				fo << it->first << ' ' << it->second << std::endl;
			}

			return true;
		}

		bool ParamsParser::hasParam(const std::string &paramName)
		{
			bool found = false;
			if (mParamsMap.find(paramName) != mParamsMap.end())
				found = true;

			return found;
		}

		double ParamsParser::getValue(const std::string &paramName, double defaultValue)
		{
			auto it = mParamsMap.find(paramName);
			if (it == mParamsMap.end())
			{
				std::cout << "Parameter '" << paramName << "' not found - using default value" << std::endl;
				return defaultValue;
			}

			return it->second;
		}

		double ParamsParser::getValue(const std::string &paramName)
		{
			auto it = mParamsMap.find(paramName);
			if (it == mParamsMap.end())
				std::cout << "ParamsParser: missing required parameter '" << paramName << "'" << std::endl;

			assert(it != mParamsMap.end());
			return it->second;
		}
	}
}
