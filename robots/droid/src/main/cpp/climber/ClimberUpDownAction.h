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
            ClimberUpDownAction(Climber &subsystem, double liftp, double travp) ;
            ClimberUpDownAction(Climber &subsystem, const std::string &lift, const std::string &trav) ;
            ClimberUpDownAction(Climber &subsystem, const std::string &lift, double trav) ;
            ClimberUpDownAction(Climber &subsystem, double lift, const std::string &trav) ;                        
            virtual ~ClimberUpDownAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual std::string toString() ;

        private:
            std::shared_ptr<xero::base::SingleMotorPowerAction> action_ ;
            double traverse_ ;
        } ;
    }
}