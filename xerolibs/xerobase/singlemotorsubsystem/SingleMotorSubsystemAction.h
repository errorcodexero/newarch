#pragma once

#include "SingleMotorSubsystem.h"
#include "actions/Action.h"
#include "Robot.h"
#include "basegroups.h"


/// \file


namespace xero {
    namespace base {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class SingleMotorSubsystemAction : public GenericAction {
        public:
            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            SingleMotorSubsystemAction(SingleMotorSubsystem &subsystem) : GenericAction(subsystem.getRobot().getMessageLogger()), subsystem_(subsystem) {
            }

        protected:
            /// \brief return the subsystem assocaited with this action
            /// \returns the subsystem associated with the action
            SingleMotorSubsystem &getSubsystem() {
                return subsystem_ ;
            }

            /// \brief set the motor power
            /// \param v the power for the motor
            void setMotor(double v) {
                subsystem_.setMotor(v) ;
            }

            uint64_t getID() {
                return subsystem_.msg_id_ ;
            }

        private:
            /// The tank drive subsystem
            SingleMotorSubsystem &subsystem_;
        };
    }
}
