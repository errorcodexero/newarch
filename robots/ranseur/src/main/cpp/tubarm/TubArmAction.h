#pragma once

#include <actions/Action.h>

namespace xero {
    namespace ranseur {
        class TubArm ;

        class TubArmAction : public xero::base::Action {
        public:
            TubArmAction(TubArm &tubarm) : tubarm_(tubarm) {                
            }

            virtual ~TubArmAction() {
            }

            TubArm &getTubArm() {
                return tubarm_ ;
            }

        private:
            TubArm &tubarm_ ;
        } ;
    }
}
