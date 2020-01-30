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

        private:
            enum class State
            {
                WaitForBall,
                WaitForSecondOff,
                WaitForSecondOn
            } ;

        private:
            State state_ ;

            double to_shooter_ ;
            double to_collecter_ ; ;
        } ;
    }
}