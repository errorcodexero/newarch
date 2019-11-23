#pragma once

#include "Subsystem.h" 
#include "SingleMotorSubsystemAction.h" 
#include <iostream>

/// \file

namespace xero {
    namespace base {
        class MotorController;

        /// \brief This class  a subsystem that consists of a single motor.
        /// The motor can be a controlled via a VictorSP motor controller or
        /// via a TalonSRX motor controller.<br><br>
        /// <b>Actions:</b><br>
        ///     - SingleMotorPowerAction
        class SingleMotorSubsystem : public Subsystem {
            /// \brief Action class for this subsystem
            friend class SingleMotorSubsystemAction;

            /// \brief Action class for this subsystem          
            friend class SingleMotorPowerAction ;

        private:
            constexpr static double epsilon = 1e-3 ;

        public:
            /// \brief Create a new subsystem object
            /// The motor parameter is the number of the PWM to use for VictorSP or the CAN address
            /// of the controller for a TalonSRX.  The motor parameter is a name that used to look up
            /// the motor number from the settings file.
            /// \param robot a reference to the robot for this subsystem
            /// \param name the name of this subsystem, must be unique across all subsystems
            /// \param motor the name of the parameter file entry that contains the motor definition
            /// \param id the message logger id to use for messages from this class            
            /// \sa xero::base::MotorFactory
            /// \sa xero::misc::SettingsParser
            SingleMotorSubsystem(Subsystem *parent, const std::string &name, const std::string &motor, uint64_t id) ;

            /// \brief Create a new subsystem object
            /// The motor parameter is the number of the PWM to use for VictorSP or the CAN address
            /// of the controller for a TalonSRX. 
            /// \param robot a reference to the robot for this subsystem
            /// \param name the name of this subsystem, must be unique across all subsystems
            /// \param motor the motor controller
            /// \param id the message logger id to use for messages from this class
            /// \sa xero::misc::SettingsParser
            SingleMotorSubsystem(Subsystem *parent,
                                 const std::string &name, 
                                 std::shared_ptr<MotorController> motor, 
                                 uint64_t id
            ) ;

            /// \brief destroy the subsystem, freeing up the motor controllers
            virtual ~SingleMotorSubsystem();

            virtual bool canAcceptAction(ActionPtr action) ;

            /// \brief returns true if the motor is running
            /// \returns true if the motor is running
            bool isRunning() const {
                return current_power_ > epsilon ;
            }

            /// @return This subsystem's message ID.
            uint64_t getMsgID() { return msg_id_; }

        protected:
            /// \brief set the power (PWM) percentage for the motor.
            /// \param power the power to apply to the motor, between -1.0 and 1.0
            void setMotor(double power);

        private:
            // The TalonSRX motor controller is a talon is used
            std::shared_ptr<MotorController> motor_;

            // The current power applied to the motor (-1 to 1)
            double current_power_ ;

            // The message id
            uint64_t msg_id_ ;
        }  ;
    }
}
