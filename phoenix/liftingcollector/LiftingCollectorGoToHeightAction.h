#pragma once
#include "LiftingCollectorAction.h"
#include "PIDCtrl.h"

namespace xero {
    namespace phoenix {
        class LiftingCollectorGoToHeightAction : public LiftingCollectorAction {
        public:
            LiftingCollectorGoToHeightAction(LiftingCollector& lifter, double target_height);
            virtual void start();
            virtual void run();
            virtual bool isDone();
            virtual void cancel();
            virtual std::string toString();
        private:
            bool is_done_;
            double target_height_;
            double current_height_;
            xero::misc::PIDCtrl PID_controller_;
        };
    };
}