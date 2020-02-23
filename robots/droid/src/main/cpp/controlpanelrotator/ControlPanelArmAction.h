#pragma once
#include "ControlPanelAction.h"

namespace xero {
    namespace droid {
        /// Deploys the control panel arm servo to a given position.
        class ControlPanelArmAction: public ControlPanelAction {
        public:
            ControlPanelArmAction(ControlPanelRotator &sub, bool up):
                ControlPanelAction(sub), up_(up) {}

            void start() override {
                getSubsystem().getArmServo()->Set(up_ ? getSubsystem().getArmUpPosition() : getSubsystem().getArmDownPosition());
                setDone();
            }

            std::string toString() override {
                std::string result = "ControlPanelArmAction: ";
                result += (up_ ? "up": "down");
                return result;
            }

        private:
            bool up_;
        };
    }
}