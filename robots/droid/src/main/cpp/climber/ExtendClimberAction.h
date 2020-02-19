#pragma once

#include "Climber.h"
#include "ClimberAction.h"
#include <TrapezoidalProfile.h>
#include <PIDACtrl.h>

namespace xero {
    namespace droid {
           class ExtendClimberAction : public ClimberAction {
  
        public:
            ExtendClimberAction(Climber &climber) ;

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;


        private:
            enum class State
            {
                ReleasingServos,
                ExtendingClimber,
                Done
            } ;
        
        private:        
            xero::base::ActionPtr release_action_ ;
            State state_ ;
            double start_ ;
            double servo_left_ ;
            double servo_right_ ;
            double servo_delay_ ;
        };
    }
}
