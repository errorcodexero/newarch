#pragma once

#include "Action.h"
#include "basegroups.h"


/// \file


namespace xero {
    namespace base {
        class SingleMotorSubsystem;

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class SingleMotorSubsystemAction : public Action {
        public:
            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            SingleMotorSubsystemAction(SingleMotorSubsystem &subsystem) : subsystem_(subsystem) {
            }

        protected:
            /// \brief return the subsystem assocaited with this action
            /// \returns the subsystem associated with the action
            SingleMotorSubsystem &getSubsystem() {
                return subsystem_ ;
            }

        private:
            /// The tank drive subsystem
            SingleMotorSubsystem &subsystem_;
        };
    }
}
