#pragma once

#include "Intake.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero
{
    namespace droid
    {
        class IntakeAction : public xero::base::Action
        {
        public:
            IntakeAction(Intake &intake) : xero::base::Action(intake.getRobot().getMessageLogger()), intake_(intake)
            {
            }

            virtual ~IntakeAction() 
            {                
            }

        protected:
            Intake& getSubsystem() {
                return intake_ ;
            }

        private:
            Intake &intake_ ;
        } ;
    }
}