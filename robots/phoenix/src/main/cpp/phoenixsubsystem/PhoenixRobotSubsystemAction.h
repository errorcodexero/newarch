#pragma once

#include <Action.h>

namespace xero {
    namespace phoenix {
        class PhoenixRobotSubsystem ;

        class PhoenixRobotSubsystemAction : public xero::base::Action {
        public:
            PhoenixRobotSubsystemAction(PhoenixRobotSubsystem &col) : robot_subsystem_(col) {                
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
