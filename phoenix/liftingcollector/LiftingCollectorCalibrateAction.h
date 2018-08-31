#pragma once
#include "LiftingCollectorAction.h"

namespace xero {
    namespace phoenix {
        class LiftingCollectorCalibrateAction : public LiftingCollectorAction {
        public:
            LiftingCollectorCalibrateAction(LiftingCollector& lifter) : LiftingCollectorAction(lifter) {

            };
            virtual void start();
            virtual void run();
            virtual bool isDone();
            virtual void cancel();
            virtual std::string toString();
        private:
            bool is_done_;
            static const std::string action_name_;
        };
    };
}