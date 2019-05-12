#pragma once

#include <string>

namespace xero {
	namespace paths {
		class ICsv
		{
		public:
			virtual std::string toCSV() const = 0;
		};
	}
}
