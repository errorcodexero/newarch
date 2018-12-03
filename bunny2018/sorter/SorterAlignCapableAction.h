#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include "SorterHoleCalculator.h"
#include <PIDCtrl.h>
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
			void setTargetAngle(double a) ;
			double getTargetAngle() const {
				return target_angle_ ;
			}

			size_t getHoleCount() const {
				return calc_->size() ;
			}

		private:
			bool isAtTarget() ;

        private:
			std::shared_ptr<SorterHoleCalculator> calc_ ;
			double target_angle_ ;
			double hole_tolerance_ ;
			xero::misc::PIDCtrl pid_ ;
        };
    }
}