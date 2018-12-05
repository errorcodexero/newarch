#include "SorterHoleCalculator.h"
#include <xeromath.h>
#include <cmath>
#include <limits>
#include <cstddef>
#include <iostream>

namespace xero {
	namespace bunny2018 {
		SorterHoleCalculator::SorterHoleCalculator(int count, double offset)
		{
			for(int i = 0 ; i < count ; i++)
				holes_.push_back(xero::math::normalizeAngleDegrees(i * 360.0 / count + offset)) ;
		}

		std::string SorterHoleCalculator::toString() {
			std::string ret ;

			for(size_t i = 0 ; i < holes_.size() ; i++) {
				if (i != 0)
					ret += " " ;
				ret += std::to_string(holes_[i]) ;
			}

			return ret ;
		}

		double SorterHoleCalculator::calcTargetAngle(double power, double angle) {
			double ret = holes_[0] ;

			//
			// The sorter motor is stopped, find the next opening that is the closest
			//
			double diff = std::numeric_limits<double>::max() ;
			for(size_t i = 0 ; i < holes_.size() ; i++) {
				double a = holes_[i] ;
				double mydiff = std::fabs(a - angle) ;
				if (mydiff < diff) {
					ret = a ;
					diff = mydiff ;
				}
			}

			std::cout << "calcTargetAngle: angle " << angle << ", result " << ret << std::endl ;

			return ret ;
		}		
	}
}