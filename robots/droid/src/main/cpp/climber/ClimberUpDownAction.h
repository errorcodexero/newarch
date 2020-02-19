#pragma once

#include "ClimberAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>

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
            std::shared_ptr<xero::base::SingleMotorPowerAction> action_ ;
        } ;
    }
}