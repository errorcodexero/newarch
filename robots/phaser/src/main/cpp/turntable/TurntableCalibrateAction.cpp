#include "TurntableCalibrateAction.h"

namespace xero {
    namespace phaser {

        std::string TurntableCalibrateAction::action_name("TurntableCalibrate") ;

        TurntableCalibrateAction::TurntableCalibrateAction(Turntable &Turntable) : TurntableAction(Turntable) {            
        }

        TurntableCalibrateAction::~TurntableCalibrateAction() {            
        }
    }
}