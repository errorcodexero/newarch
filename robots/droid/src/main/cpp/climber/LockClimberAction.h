#pragma once

#include "ClimberAction.h"

namespace xero
{
    namespace droid
    {
        class LockClimberAction : public ClimberAction
        {
        public:
            LockClimberAction(Climber &subsystem) ;
            virtual ~LockClimberAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;

        private:
            double servo_left_ ;
            double servo_right_ ;
            double servo_delay_ ;
            double start_ ;
        } ;
    }
}
