#pragma once

#include "Shooter.h"
#include <actions/Action.h>

namespace xero {
    namespace droid {
        class ShooterAction: public xero::base::Action {
        public:
            ShooterAction(Shooter &sub): 
                Action(sub.getRobot().getMessageLogger()), 
                subsystem_(sub) {}

            Shooter &getSubsystem() { return subsystem_; }
            
        private:
            Shooter &subsystem_;
        };
    }
}