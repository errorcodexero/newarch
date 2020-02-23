#pragma once
#include "ControlPanelAction.h"

namespace xero {
    namespace droid {
        /// Deploys the control panel arm servo to a given position.
        class ControlPanelArmAction: public ControlPanelAction {
            ControlPanelArmAction(ControlPanelRotator &sub, double position): 
                ControlPanelAction(sub), position_(position) {}

            ControlPanelArmAction(ControlPanelRotator &sub, bool up):
                ControlPanelAction(sub), position_(up ? sub.getArmUpPosition() : sub.getArmDownPosition()) {}

            void start() override {
                getSubsystem().getArmServo()->Set(position_);
                setDone();
            }

            void toString() override {
                return "ControlPanelArmAction: " + (up ? "up" : "down");
            }

        private:
            double position_;
        };
    }
}