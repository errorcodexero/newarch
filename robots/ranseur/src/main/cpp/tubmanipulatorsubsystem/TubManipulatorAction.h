#pragma once

#include <actions/Action.h>
#include <actions/DispatchAction.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {
        class TubManipulatorSubsystem ;

        class TubManipulatorAction : public xero::base::Action {

        public:

            TubManipulatorAction(TubManipulatorSubsystem &tubmanipulatorsubsystem) : tubmanipulator_(tubmanipulatorsubsystem) {                
            }

            virtual ~TubManipulatorAction() {                
            }

            TubManipulatorSubsystem &getTubManipulator() {
                return tubmanipulator_ ;
            }      

        private:
            TubManipulatorSubsystem &tubmanipulator_ ;
        };
    }
}
