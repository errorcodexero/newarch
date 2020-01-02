#pragma once

#include "tubmanipulatorsubsystem/TubManipulatorSubsystem.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace ranseur {
        class TubManipulatorSubsystem ;

        class TubManipulatorAction : public xero::base::GenericAction {

        public:

            TubManipulatorAction(TubManipulatorSubsystem &tubmanipulatorsubsystem) 
                        : GenericAction(tubmanipulatorsubsystem.getRobot().getMessageLogger()), tubmanipulator_(tubmanipulatorsubsystem) {
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
