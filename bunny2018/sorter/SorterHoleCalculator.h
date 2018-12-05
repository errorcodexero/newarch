#pragma once

#include <vector>
#include <string>

namespace xero
{
	namespace bunny2018
	{
		class SorterHoleCalculator
		{
		public:
			SorterHoleCalculator(int count, double offset) ;

			double calcTargetAngle(double power, double angle) ;

			std::size_t size() const {
				return holes_.size() ;
			}

			std::string toString() ;

		private:
			std::vector<double> holes_ ;
		} ;
	}
}