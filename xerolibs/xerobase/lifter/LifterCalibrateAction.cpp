#include "LifterCalibrateAction.h"

namespace xero {
    namespace base {

        std::string LifterCalibrateAction::action_name("LifterCalibrate") ;

        LifterCalibrateAction::LifterCalibrateAction(Lifter &lifter) : LifterAction(lifter) {            
        }

        LifterCalibrateAction::~LifterCalibrateAction() {            
        }
    }
}