#pragma once

#include <Action.h>

namespace xero {
    namespace phaser {
        class Climber ;

        class ClimberAction : public xero::base::Action {
        public:
            ClimberAction(Climber &climber) : climber_(climber) {                
            }
            
            Climber &getClimber() {
                return climber_ ;
            }

        private:
            Climber &climber_ ;
        } ;
    }
}
