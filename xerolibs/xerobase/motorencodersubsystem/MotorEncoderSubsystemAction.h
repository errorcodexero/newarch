#pragma once

#include "MotorEncoderSubsystem.h"
#include "actions/Action.h"
#include "Robot.h"

namespace xero {
    namespace base {
        class MotorEncoderSubsystemAction : public GenericAction {
        public:
            MotorEncoderSubsystemAction(MotorEncoderSubsystem &subsystem) : GenericAction(subsystem.getRobot().getMessageLogger()), subsystem_(subsystem) {                
            }

            MotorEncoderSubsystem &getSubsystem() {
                return subsystem_ ;
            }

        private:
            MotorEncoderSubsystem &subsystem_ ;
        } ;
    }
}