#pragma once

#include <Action.h>

namespace xero {
    namespace phoenix {
        class PhoenixRobotSubsystem ;

        class PhoenixSubsystemAction : public xero::base::Action {
        public:
            PhoenixSubsystemAction(PhoenixRobotSubsystem &col) : robot_subsystem_(col) {                
            }

            virtual ~PhoenixSubsystemAction() {                
            }

            PhoenixRobotSubsystem &getRobotSubsystem() {
                return robot_subsystem_ ;
            }

        private:
            PhoenixRobotSubsystem &robot_subsystem_ ;
        } ;
    }
}
