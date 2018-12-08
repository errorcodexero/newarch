#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterRotateAngleAction : public SorterAction {
        public:
            SorterRotateAngleAction(Sorter &sorter, double angle, double power) ;

            virtual ~SorterRotateAngleAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;

		private:
			void calcNextAngle() ;

		private:
			static constexpr double epsilon = 1e-3 ;
            
        private:
			double target_angle_ ;
			double power_ ;
			double total_angle_ ;
			double next_angle_ ;
			double next_angle_size_ ;
			bool is_done_ ;
        };
    }
}