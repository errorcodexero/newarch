#pragma once

#include "droidids.h"
#include "Climber.h"
#include "ClimberAction.h"

namespace xero {
    namespace droid {
        class Climber;
        class ReleaseAction : public ClimberAction {
            //motor encoder system w/ 2 servos
        
        public:
            ReleaseAction(Climber &climber) : ClimberAction(climber) {}

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;
        };
    }
}
