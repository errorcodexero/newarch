#pragma once

#include "ranseurrobotsubsystem/RanseurRobotSubsystem.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace ranseur {
        class RanseurRobotSubsystem ;

        class RanseurSubsystemAction : public xero::base::GenericAction {

        public:

            RanseurSubsystemAction(RanseurRobotSubsystem &ranseurrobotsubsystem) : GenericAction(ranseurrobotsubsystem.getRobot().getMessageLogger()), subsystem_(ranseurrobotsubsystem) {
            }

            virtual ~RanseurSubsystemAction() {                
            }

            RanseurRobotSubsystem &getRanseurSubsystem() {
                return subsystem_ ;
            }      

        private:
            RanseurRobotSubsystem &subsystem_ ;
        };
    }
}
