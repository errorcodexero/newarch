#pragma once

#include <memory>
#include <motorencodersubsystem/MotorEncoderHoldAction.h>

#include "Turret.h"

namespace xero {
    namespace droid {
        class FollowTargetAction : public xero::base::MotorEncoderHoldAction {
        public:
            FollowTargetAction(Turret &subsystem);
           
            virtual void run() ;

            Turret &getTurret() { 
                return static_cast<Turret&>(getSubsystem());
            }

        private:
            double threshold_;
        };
    }
}
