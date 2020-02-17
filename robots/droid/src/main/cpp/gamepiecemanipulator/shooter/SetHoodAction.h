#pragma once

#include "ShooterAction.h"

namespace xero {
    namespace droid {
        class SetHoodAction: public ShooterAction {
        public:
            SetHoodAction(Shooter &sub, bool hood): ShooterAction(sub), hood_(hood) {}

            void start() override {
                ShooterAction::start();
                getSubsystem().setHood(hood_);
                setDone();
            }

            std::string toString() override { return "SetHoodAction(" + std::to_string(hood_) + ")"; }
            
        private:
            bool hood_;
        };
    }
}