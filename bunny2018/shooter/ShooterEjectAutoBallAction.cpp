#include "ShooterEjectAutoBallAction.h"

namespace xero {
    namespace bunny2018 {
        ShooterEjectAutoBallAction::ShooterEjectAutoBallAction(Shooter& shooter) : ShooterEjectOneBallAction(shooter) {
            db_ = shooter.getRobot().getDriveBase() ;
        }

        ShooterEjectAutoBallAction::~ShooterEjectAutoBallAction() {
        }

        void ShooterEjectAutoBallAction::start() {
            base_location_ = db_->getDist() ;
            filled_.resize(crates_.size()) ;
            std::fill(filled_.begin(), filled_.end(), false) ;
        }

        bool ShooterEjectAutoBallAction::ejectTrigger() {
            return false ;
        }

        bool ShooterEjectAutoBallAction::repeatEjectOne() {
            return true ;
        }
    }
}