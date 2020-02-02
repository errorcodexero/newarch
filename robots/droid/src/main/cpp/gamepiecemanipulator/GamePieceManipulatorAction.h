#pragma once

#include <actions/Action.h>

namespace xero
{
    namespace droid
    {
        class GamePieceManipulator ;
        
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
