#include "ShootAction.h"
#include "shooter/FireAction.h"
#include <gamepiecemanipulator/shooter/Shooter.h>
#include <gamepiecemanipulator/GamePieceManipulator.h>

namespace xero {
    namespace droid {

        ShootAction::ShootAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem)
        {
        }

        ShootAction::~ShootAction()
        {

        }

        void ShootAction::start() 
        {
        }

        void ShootAction::run() 
        {
        }
    }
}