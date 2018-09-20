#pragma once
#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterChangeGearAction : public LifterAction {
        public:
            LifterChangeGearAction(Lifter& lifter, Lifter::LIFTER_GEAR value);
            virtual void start();
            virtual void run();
            virtual bool isDone();
            virtual void cancel();
            virtual std::string toString();
        private:
            Lifter::LIFTER_GEAR value_;
            static const std::string action_name_;
        };
    };
}