#include "LiftingCollectorCalibrateAction.h"
#include "Robot.h"
#include <cassert>

//const double BOTTOM_HEIGHT = 11.375;

namespace xero {
    namespace phoenix {
        void LiftingCollectorCalibrateAction::start() {
            getLiftingCollector().calibrate();
        }

        void LiftingCollectorCalibrateAction::run() {
        }

        bool LiftingCollectorCalibrateAction::isDone() {
            return true;
        }

        void LiftingCollectorCalibrateAction::cancel() {
        }
		
        std::string LiftingCollectorCalibrateAction::toString() {
            return action_name_;
        }
        const std::string LiftingCollectorCalibrateAction::action_name_("CalibrateLiftingCollector");
    }
}