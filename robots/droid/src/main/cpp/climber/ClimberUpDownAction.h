#pragma once

#include "ClimberAction.h"

namespace xero
{
    namespace droid
    {
        class ClimberUpDownAction : public ClimberAction
        {
        public:
            ClimberUpDownAction(Climber &subsystem, double p) ;        
            ClimberUpDownAction(Climber &subsystem, const std::string &name) ;
            virtual ~ClimberUpDownAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;

        private:
            xero::base::ActionPtr action_ ;
        } ;
    }
}