#pragma once

#include "ConveyorAction.h"
#include "Conveyor.h"

namespace xero {
    namespace droid {
        /// An action which ejects all balls through the intake.
        class ConveyorSetBallAction : public ConveyorAction {
        public:
            ConveyorSetBallAction(Conveyor &subsystem, int n) : ConveyorAction(subsystem)
            {
                count_ = n ;
            }

            void start() override
            {
                getSubsystem().setBallCount(count_) ;
            }

            void run() override
            {
                setDone() ;
            }

            void cancel() override
            {
                setDone() ;
            }

            std::string toString() override
            {
                std::string ret = "ConveyorSetBallAction " + std::to_string(count_) ;
                return ret ;
            }

        private:
            int count_ ;

        } ;
    }
}