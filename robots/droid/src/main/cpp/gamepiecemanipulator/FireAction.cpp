#include "FireAction.h"
#include "shooter/ShooterVelocityAction.h"

namespace xero {
    namespace droid {
        void FireAction::start() {
            auto &manip = getSubsystem() ;
            manip.getShooter()->setAction(std::make_shared<ShooterVelocityAction>(*manip.getShooter()), /*allowParentBusy=*/true) ;
        }

        void FireAction::run() {
        }
    }
}