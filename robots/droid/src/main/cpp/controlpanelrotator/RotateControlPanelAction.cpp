#include "RotateControlPanelAction.h"

namespace xero {
    namespace droid {
        void RotateControlPanelAction::start() {
            lastColor_ = getRotatorSubsystem().getLastColor();
            wedgesRemaining_ = getTotalWedges();
            updateTarget();
        }

        void RotateControlPanelAction::run() {
            ControlPanelRotator::Color currentColor = getRotatorSubsystem().getLastColor();

            if (currentColor != lastColor_) {
                wedgesRemaining_ -= ControlPanelRotator::distanceToColor(lastColor_, currentColor);
                lastColor_ = currentColor;
                updateTarget();
            }
        }

        void RotateControlPanelAction::updateTarget() {
            // Aim for the center of the target wedge

            const float DIST_PER_WEDGE = 1;
            float dist = (wedgesRemaining_ + 0.5)*DIST_PER_WEDGE;

            setTarget(getSubsystem().getPosition() + dist);
        }
    }
}