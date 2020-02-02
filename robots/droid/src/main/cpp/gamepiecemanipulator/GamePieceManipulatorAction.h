#pragma once

#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <actions/Action.h>
#include <Robot.h>

namespace xero
{
    namespace droid
    {
        class GamePieceManipulatorAction : public xero::base::Action
        {
        public:
            GamePieceManipulatorAction(GamePieceManipulator &subsystem) : xero::base::Action(subsystem.getRobot().getMessageLogger()), subsystem_(subsystem)
            {                
            }

            virtual ~GamePieceManipulatorAction()
            {
            }

            GamePieceManipulator &getSubsystem() 
            {
                return subsystem_ ;
            }

        private:
            GamePieceManipulator &subsystem_ ;
        };
    }
}
