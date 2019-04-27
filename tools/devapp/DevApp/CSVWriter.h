#pragma once
#include "ICsv.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace xero
{
	namespace paths
	{
		class CSVWriter
		{
		public:
			CSVWriter() = delete;
			~CSVWriter() = delete;

			template<class InputIt>
			static bool write(const std::string& filename, InputIt first, InputIt last)
			{
				std::ofstream strm(filename);
				if (!strm.is_open())
				{
					std::cerr << "CSVWriter: could not open file '" << filename << "' for writing" << std::endl;
					return false;
				}

				for (auto it = first; it != last; it++)
				{
					const ICsv &cl = *it;
					strm << cl.toCSV() << std::endl;
				}

				return true;
			}
		};
	}
}


