#include <memory>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

#include "ControlPanelRotator.h"

namespace xero {
    namespace droid {
        class RotateControlPanelAction: public xero::base::MotorEncoderGoToAction {
            // Creates an action to rotate the control panel.
            RotateControlPanelAction(ControlPanelRotator &subsystem, const std::string &configName): 
                MotorEncoderGoToAction(subsystem, 0, configName) {}

            ControlPanelRotator &getRotatorSubsystem() {
                return static_cast<ControlPanelRotator>(getSubsystem());
            }

            void start() override;
            void run() override;

        protected:
            // Called when the action starts to calculate the total number of wedges
            // to travel from the current (starting) position to the ending position.
            virtual int getTotalWedges();

        private:
            void updateTarget();

            int wedgesRemaining_;
            ControlPanelRotator::Color lastColor_ = ControlPanelRotator::Color::Blue;
        }
    }
}