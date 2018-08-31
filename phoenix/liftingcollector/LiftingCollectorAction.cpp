#include "LiftingCollectorAction.h"

namespace xero {
    namespace phoenix {
        LiftingCollectorAction::LiftingCollectorAction(LiftingCollector& lifter) : lifter_(lifter) {

        };
        LiftingCollector& LiftingCollectorAction::getLiftingCollector() {
            return lifter_;
        }
    }
}