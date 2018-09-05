#pragma once
#include "Action.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class Lifter;
        class LifterAction : public xero::base::Action {
        public:
            LifterAction(Lifter& lifter);
            Lifter& getLifter();
        private:
            Lifter& lifter_;
        };
    };
}