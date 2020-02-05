#pragma once

#include "GamePieceManipulator.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero
{
    namespace droid
    {
        class GamePieceManipulatorAction : public xero::base::Action
        {
        public:
            GamePieceManipulatorAction(GamePieceManipulator &subsystem) : 
                xero::base::Action(subsystem.getRobot().getMessageLogger()),
                manip_(subsystem)
            {
            }

            virtual ~GamePieceManipulatorAction()
            {
            }

        protected:
            GamePieceManipulator &getSubsystem() {
                return manip_ ;
            }

        private:
            GamePieceManipulator &manip_ ;
        } ;
    }
}
