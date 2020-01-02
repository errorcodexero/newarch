#pragma once

#include "bunnyarm/BunnyArm.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace ranseur {
        class BunnyArm ;

        class BunnyArmAction : public xero::base::GenericAction {
        public:
            BunnyArmAction(BunnyArm &bunnyArm) : GenericAction(bunnyArm.getRobot().getMessageLogger()), bunnyArm_(bunnyArm) {
            }
            
            BunnyArm &getBunnyArm() {
                return bunnyArm_ ;
            }

        private:
            BunnyArm &bunnyArm_ ;
        } ;
    }
}
