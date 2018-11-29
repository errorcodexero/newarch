#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterDutyCycleAction : public SorterAction {
        public:
            enum Which
            {
                IntakeMotor,
                OuttakeMotor,
                SortMotor
            } ;

            SorterDutyCycleAction(Sorter &sorter, Which w, double duty) ;

            SorterDutyCycleAction(Sorter &sorter, Which w, const std::string &name) ;

            virtual ~SorterDutyCycleAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
            Which which_ ;
            double duty_cycle_;
        };
    }
}