#include "FireAction.h"
#include "shooter/ShooterVelocityAction.h"

namespace xero {
    namespace droid {
        void FireAction::start() {
            auto &manip = getSubsystem() ;
            shooterVelocityAction_ = std::make_shared<ShooterVelocityAction>(*manip.getShooter());
        }

        void FireAction::run() {
        }
    }
}