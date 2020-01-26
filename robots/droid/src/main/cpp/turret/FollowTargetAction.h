#pragma once

#include <memory>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

#include "Turret.h"

namespace xero {
    namespace droid {
        class FollowTargetAction : public xero::base::MotorEncoderGoToAction {
        public:
            
            FollowTargetAction(Turret &subsystem, const std::string &configName): 
                MotorEncoderGoToAction(subsystem, 0, configName) {}
           
            virtual void run() ;

        };
    }
}
