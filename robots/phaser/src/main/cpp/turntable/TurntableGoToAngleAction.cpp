#include "TurntableGoToAngleAction.h"
#include "Turntable.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        TurntableGoToAngleAction::TurntableGoToAngleAction(Turntable &turntable, double target) : TurntableAction(turntable) {
            target_ = target ;
        }

        TurntableGoToAngleAction::TurntableGoToAngleAction(Turntable &turntable, const std::string &name) : TurntableAction(turntable) {
            target_ = getTurntable().getRobot().getSettingsParser().getDouble(name) ;
        }

        TurntableGoToAngleAction::~TurntableGoToAngleAction() {
        }

        void TurntableGoToAngleAction::start() {
            start_time_ = getTurntable().getRobot().getTime() ;
        }

        void TurntableGoToAngleAction::run() {
            Turntable &turntable = getTurntable() ;
            MessageLogger &logger = turntable.getRobot().getMessageLogger() ;
            double dt = turntable.getRobot().getDeltaTime() ;
        }

        bool TurntableGoToAngleAction::isDone() {
            return is_done_ ;
        }

        void TurntableGoToAngleAction::cancel() {
            is_done_ = true ;
        }

        std::string TurntableGoToAngleAction::toString() {
            std::string result = "TurntableGoToAngle " + std::to_string(target_) ;
            return result ;
        }
    }
}