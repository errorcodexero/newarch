#pragma once

#include "ConveyorAction.h"

namespace xero {
    namespace droid {
        class ConveyorEmitAction : public ConveyorAction {
        public:
            ConveyorEmitAction(Conveyor &subsystem);
            virtual ~ConveyorEmitAction() ;

            virtual void start() ;
            virtual void run() ;

            virtual std::string toString() {
                return "emit" ;
            }

        private:
            enum class State
            {
                WaitForBall,
                WaitForSecondOff,
                WaitForSecondOn,
                WaitForSecondDelay,
                Done
            } ;

        private:
            State state_ ;

            double start_ ;
            double delay1_ ;
            double to_shooter_ ;
            double to_collecter_ ;
        } ;
    }
}