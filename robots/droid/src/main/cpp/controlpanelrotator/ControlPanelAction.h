#pragma once
#include "ControlPanelRotator.h"
#include <actions/Action.h>

namespace xero {
    namespace droid {
        class ControlPanelAction: public xero::base::Action {
        public:
            ControlPanelRotator &getSubsystem() { return sub_; }
        protected:
            ControlPanelAction(ControlPanelRotator &subsystem): 
                xero::base::Action(subsystem.getRobot().getMessageLogger()), sub_(subsystem) {}
        private:
            ControlPanelRotator &sub_;
        };
    }
}