#pragma once

#include "RotateControlPanelAction.h"

namespace xero {
    namespace droid {
        class ControlPanelDistanceAction: public RotateControlPanelAction {
            // Creates an action to rotate the control panel.
            ControlPanelDistanceAction(ControlPanelRotator &subsystem):
                RotateControlPanelAction(subsystem, "distance") { }

            virtual int getTotalWedges() { 
                std::string configName = getSubsystem().getConfigName() + ":distance";
                return getSubsystem().getRobot().getSettingsParser().getDouble(configName);
            }
        };
    }
}