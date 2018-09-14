#pragma once
#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterMaintainHeightAction : public LifterAction {
        public:
            LifterMaintainHeightAction(Lifter& lifter, Lifter::LIFTER_GEAR value);
            virtual void start();
            virtual void run();
            virtual bool isDone();
            virtual void cancel();
            virtual std::string toString();
        private:
            enum class LIFTER_STATE {HOLDING, CLIMBING};
            LIFTER_STATE state_;
            int encoder_value_target_;
            static const std::string action_name_;
            int climb_delta_;
            Lifter::LIFTER_GEAR value_;
        };
    };
}