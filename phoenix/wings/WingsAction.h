#pragma once

#include <Action.h>

namespace xero {
    namespace phoenix {
        class Wings ;

        class WingsAction : public xero::base::Action {
        public:
            WingsAction(Wings &wings) : wings_(wings) {                
            }
            
            virtual bool getDeploy() = 0 ;

            Wings &getIntake() {
                return wings_ ;
            }

        private:
            Wings &wings_ ;
        } ;
    }
}
