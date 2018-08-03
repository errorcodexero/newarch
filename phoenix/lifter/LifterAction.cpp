#include "LifterAction.h"

namespace xero {
    namespace phoenix {
        LifterAction::LifterAction(Lifter& lifter) : lifter_(lifter) {

        };
        Lifter& LifterAction::getLifter() {
            return lifter_;
        }
    }
}