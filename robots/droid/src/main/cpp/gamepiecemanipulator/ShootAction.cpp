#include "ShootAction.h"
#include "shooter/ShooterVelocityAction.h"

namespace xero {
    namespace droid {
        void ShootAction::start() {
            auto &manip = getSubsystem() ;
            manip.getShooter()->setAction(std::make_shared<ShooterVelocityAction>(*manip.getShooter()), /*allowParentBusy=*/true) ;
        }

        void ShootAction::run() {
        }
    }
}