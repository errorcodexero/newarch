#pragma once

#include "Conveyor.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero
{
    namespace droid
    {
        class ConveyorAction : public xero::base::Action
        {
        public:
            ConveyorAction(Conveyor &subsystem) : xero::base::Action(subsystem.getRobot().getMessageLogger()), conveyor_(subsystem)
            {
            }

            virtual ~ConveyorAction()
            {                
            }

        protected:
            Conveyor &getSubsystem() {
                return conveyor_ ;
            }

        private:
            Conveyor &conveyor_ ;
        } ;
    }
}