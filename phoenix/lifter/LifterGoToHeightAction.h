#pragma once
#include "LifterAction.h"
#include "PIDCtrl.h"

namespace xero {
    namespace phoenix {
        class LifterGoToHeightAction : public LifterAction {
        public:
            LifterGoToHeightAction(Lifter& lifter, double target_height);
			LifterGoToHeightAction(Lifter &lifter, const std::string &name) ;
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