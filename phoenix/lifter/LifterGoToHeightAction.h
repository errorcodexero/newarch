#pragma once
#include "LifterAction.h"

namespace xero {
    namespace phoenix {
        class LifterGoToHeightAction : public LifterAction {
        public:
            LifterGoToHeightAction(Lifter& lifter) : LifterAction(lifter) {

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