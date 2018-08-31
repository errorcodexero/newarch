#pragma once
#include "Action.h"
#include "LiftingCollector.h"

namespace xero {
    namespace phoenix {
        class LiftingCollector;
        class LiftingCollectorAction : public xero::base::Action {
        public:
            LiftingCollectorAction(LiftingCollector& lifter);
            LiftingCollector& getLiftingCollector();
        private:
            LiftingCollector& lifter_;
        };
    };
}