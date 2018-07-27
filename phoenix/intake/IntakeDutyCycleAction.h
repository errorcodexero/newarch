#pragma once

#include "IntakeAction.h" 

namespace xero {
    namespace phoenix {
        class Intake ;

        class IntakeDutyCycleAction : public IntakeAction {
        public:
            IntakeDutyCycleAction(Intake &intake, double v) ;
            virtual ~IntakeDutyCycleAction() ;
            virtual void start() ;
            virtual void end() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual bool cancel() ;
            virtual std::string toString() ;
        private:
            double duty_cycle_;
        };
    }
}