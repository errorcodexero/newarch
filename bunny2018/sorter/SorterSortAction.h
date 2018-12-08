#pragma once

#include "Sorter.h"
#include "SorterAction.h"
#include "SorterHoleCalculator.h"
#include <PIDCtrl.h>
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterSortAction : public SorterAction {
        public:
            SorterSortAction(Sorter &sorter Sorter::BallColor color) ;

            virtual ~SorterSortAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;

        private:
            typedef enum class State {
                Waiting, 
                GoToAngle
            } State;

		private:
			Sorter::BallColor color_ ;
            State state_ ;
            xero::misc::PIDCtrl ctrl_ ;
            double reject_angle_ ;
            double keep_angle_ ;
            double tolerance_ ;
            double target_ ;
            SorterHoleCalculator calc_ ;
        };
    }
}