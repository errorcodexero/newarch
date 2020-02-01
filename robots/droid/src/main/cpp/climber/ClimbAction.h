#pragma once

#include "Climber.h"
#include "ClimberAction.h"
#include "droidids.h"

namespace xero {
    namespace droid {
           class ClimbAction : public ClimberAction {
  
        public:
            ClimbAction(Climber &climber) : ClimberAction(climber) {}

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;
        };
    }
}
