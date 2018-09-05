#include "LifterCalibrateAction.h"
#include "Robot.h"
#include <cassert>

//const double BOTTOM_HEIGHT = 11.375;

namespace xero {
    namespace phoenix {
        void LifterCalibrateAction::start() {

        }
        void LifterCalibrateAction::run() {
            getLifter().calibrate();
        }
        bool LifterCalibrateAction::isDone() {
            return true;
        }

        void LifterCalibrateAction::cancel() {
        }
		
        std::string LifterCalibrateAction::toString() {
            return action_name_;
        }
        const std::string LifterCalibrateAction::action_name_("CalibrateLifter");
    }
}