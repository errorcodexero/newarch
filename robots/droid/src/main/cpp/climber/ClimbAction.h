#pragma once

#include "Climber.h"
#include "ClimberAction.h"
#include <TrapezoidalProfile.h>
#include <PIDACtrl.h>

namespace xero {
    namespace droid {
           class ClimbAction : public ClimberAction {
  
        public:
            ClimbAction(Climber &climber) ;

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;
        
        private:
        
        };
    }
}
