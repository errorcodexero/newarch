#pragma once

#include "Climber.h"

namespace xero {
    namespace droid {
        class ClimberAction : public xero::base::Action {
           public:
            ClimberAction(Climber &climber) : Action(climber.getRobot().getMessageLogger()), climber_(climber) {                
            }
            virtual ~ClimberAction() {                
            }

            Climber &getSubsystem() {
                return climber_ ;
            }

        private:
            Climber climber_ ;

        } ;
    }
}