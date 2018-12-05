#pragma once

#include "SorterAlignCapableAction.h"
#include "Sorter.h"
#include <string>
#include <vector>

namespace xero {
    namespace bunny2018 {
        class SorterStageBallAction : public SorterAlignCapableAction {
        public:
            SorterStageBallAction(Sorter &sorter, Sorter::BallColor color) ;

            virtual ~SorterStageBallAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
            enum State {
                WaitingForBall,
                KeepingBall,
                RejectingBall,
                AlignSorter,
				Done,
            } ;

        private:
            Sorter::BallColor color_ ;

            double intake_power_ ;
			double output_power ;
            double keep_ball_motor_power_ ;
            double keep_ball_duration_ ;
            double reject_ball_motor_power_ ;
            double reject_ball_duration_ ;

			double next_hole_power_ ;

            State state_ ;

            double kick_start_ ;

        };
    }
}