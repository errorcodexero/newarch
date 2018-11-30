#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>
#include <vector>

namespace xero {
    namespace bunny2018 {
        class SorterStageBallAction : public SorterAction {
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
				Done
            } ;

        private:
            bool alignSorter() ;
			double calcTargetAngle() ;

        private:
            Sorter::BallColor color_ ;

            double intake_power_ ;
			double output_power ;
            double keep_ball_motor_power_ ;
            double keep_ball_duration_ ;
            double reject_ball_motor_power_ ;
            double reject_ball_duration_ ;
			double hole_tolerance_ ;
			double next_hole_power_ ;

            State state_ ;

            double kick_start_ ;
			double target_angle_ ;

			std::vector<double> holes_ ;
        };
    }
}