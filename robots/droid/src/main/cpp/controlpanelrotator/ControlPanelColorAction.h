#pragma once

#include "RotateControlPanelAction.h"

namespace xero {
    namespace droid {
        class ControlPanelColorAction: public RotateControlPanelAction {
            // Creates an action to rotate the control panel.
            ControlPanelColorAction(ControlPanelRotator &subsystem, ControlPanelRotator::Color target):
                RotateControlPanelAction(subsystem, "distance"), target_(target) { }

            virtual int getTotalWedges() { 
                int dist = ControlPanelRotator::distanceToColor(
                    getRotatorSubsystem().getLastColor(),
                    target_
                );

                // make sure we turn at least 2 wedges so that we have time
                // to slow down after the first color transition
                if (dist < 2) {
                    dist += ControlPanelRotator::Color::COUNT;
                }

                return dist;
            }

        private:
            ControlPanelRotator::Color target_;
        };
    }
}