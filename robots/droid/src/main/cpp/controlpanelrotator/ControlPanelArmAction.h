#pragma once
#include "ControlPanelAction.h"

namespace xero {
    namespace droid {
        /// Deploys the control panel arm servo to a given position.
        class ControlPanelArmAction: ControlPanelAction {
            ControlPanelArmAction(ControlPanelRotator &sub, double position): 
                ControlPanelAction(sub), position_(position) {}

            ControlPanelArmAction(ControlPanelRotator &sub, std::string &positionConfig):
                ControlPanelAction(sub), position_(sub.getRobot().getSettingsParser().getDouble(positionConfig)) {}

            void start() override {
                getSubsystem().getArmServo()->Set(position_);
                setDone();
            }

        private:
            double position_;
        };
    }
}