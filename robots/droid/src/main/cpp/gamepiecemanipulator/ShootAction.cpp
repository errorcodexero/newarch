#include "ShootAction.h"
#include "shooter/FireAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>

namespace xero {
    namespace droid {

        ShootAction::ShootAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem)
        {
        }

        ShootAction::~ShootAction()
        {

        }

        void ShootAction::start() {
            auto &manip = getSubsystem() ;
            manip.getShooter()->setAction(std::make_shared<FireAction>(*manip.getShooter()), /*allowParentBusy=*/true) ;
        }

        void ShootAction::run() {
        }
    }
}