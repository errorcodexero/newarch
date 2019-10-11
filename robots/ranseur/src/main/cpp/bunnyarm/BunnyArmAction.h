#pragma once

#include <Action.h>

namespace xero {
    namespace phaser {
        class BunnyArm ;

        class BunnyArmAction : public xero::base::Action {
        public:
            BunnyArmAction(BunnyArm &bunnyArm) : bunnyArm_(bunnyArm) {                
            }
            
            BunnyArm &getBunnyArm() {
                return bunnyArm_ ;
            }

        private:
            BunnyArm &bunnyArm_ ;
        } ;
    }
}
