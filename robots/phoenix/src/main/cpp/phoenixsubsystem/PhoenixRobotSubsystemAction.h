#pragma once

#include "phoenixsubsystem/PhoenixRobotSubsystem.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {
        class PhoenixRobotSubsystem ;

        class PhoenixRobotSubsystemAction : public xero::base::GenericAction {
        public:
            PhoenixRobotSubsystemAction(PhoenixRobotSubsystem &col) : GenericAction(col.getRobot().getMessageLogger()), robot_subsystem_(col) {                
            }

            virtual ~PhoenixRobotSubsystemAction() {                
            }

            PhoenixRobotSubsystem &getRobotSubsystem() {
                return robot_subsystem_ ;
            }

        private:
            PhoenixRobotSubsystem &robot_subsystem_ ;
        } ;
    }
}
