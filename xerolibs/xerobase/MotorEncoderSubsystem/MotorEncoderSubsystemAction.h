#pragma once

#include <Action.h>

namespace xero {
    namespace base {
        class MotorEncoderSubsystem ;

        class MotorEncoderSubsystemAction : public xero::base::Action {
        public:
            MotorEncoderSubsystemAction(MotorEncoderSubsystem &subsystem) : subsystem_(subsystem) {                
            }

            MotorEncoderSubsystem &getSubsystem() {
                return subsystem_ ;
            }

        private:
            MotorEncoderSubsystem &subsystem_ ;
        } ;
    }
}