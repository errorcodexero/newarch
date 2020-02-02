#include "ShootAction.h"
#include "shooter/FireAction.h"

namespace xero {
    namespace droid {
        void ShootAction::start() {
            auto &manip = getSubsystem() ;
            manip.getShooter()->setAction(std::make_shared<FireAction>(*manip.getShooter()), /*allowParentBusy=*/true) ;
        }

        void ShootAction::run() {
        }
    }
}