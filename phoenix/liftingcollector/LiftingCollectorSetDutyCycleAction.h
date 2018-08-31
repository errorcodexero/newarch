#pragma once
#include "LiftingCollectorAction.h"

namespace xero {
    namespace phoenix {
        class LiftingCollectorSetDutyCycleAction : public LiftingCollectorAction {
        public:
            LiftingCollectorSetDutyCycleAction(LiftingCollector& lifter, double value);
            virtual void start();
            virtual void run();
            virtual bool isDone();
            virtual void cancel();
            virtual std::string toString();
        private:
            double value_;
            bool is_done_;
            static const std::string action_name_;
        };
    };
}