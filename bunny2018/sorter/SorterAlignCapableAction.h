#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterAlignCapableAction : public SorterAction {
        public:
            SorterAlignCapableAction(Sorter &sorter) ;

            virtual ~SorterAlignCapableAction() ;

		protected:
			double calcTargetAngle() ;		
			bool alignSorter() ;
			void setTargetAngle(double a) {
				target_angle_ = a ;
			}
            
        private:
			double target_angle_ ;
			std::vector<double> holes_ ;
			double hole_tolerance_ ;
			double next_hole_power_ ;			
        };
    }
}