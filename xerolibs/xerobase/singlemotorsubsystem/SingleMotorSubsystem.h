#pragma once

#include "Subsystem.h" 
#include "SingleMotorSubsystemAction.h" 
#include <ctre/Phoenix.h>
#include <iostream>

/// \file

/// \brief a short cut type for specifying a TalonSRX motor controller
typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;
typedef ctre::phoenix::motorcontrol::can::VictorSPX VictorSPX;
typedef ctre::phoenix::motorcontrol::IMotorController IMotorController ;

namespace xero {
    namespace base {

        /// \brief This class is a subsystem that consists of a single motor.
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
            /// \param motor the name of the parameter file entry that contains the index of the motor
            /// \param id the message logger id to use for messages from this class            
            /// \param victor if true use a VictorSP motor controller, otherwise create a TalonSRX
            /// \sa xero::misc::SettingsParser
            SingleMotorSubsystem(Robot &robot, const std::string &name, const std::string &motor, uint64_t id, bool victor=false) ;

            /// \brief Create a new subsystem object
            /// The motor parameter is the number of the PWM to use for VictorSP or the CAN address
            /// of the controller for a TalonSRX. 
            /// \param robot a reference to the robot for this subsystem
            /// \param name the name of this subsystem, must be unique across all subsystems
            /// \param motor the actual number (PWM or CAN address) for the motor controller
            /// \param id the message logger id to use for messages from this class
            /// \param victor if true use a VictorSP motor controller, otherwise create a TalonSRX
            /// \sa xero::misc::SettingsParser
            SingleMotorSubsystem(Robot &robot, const std::string &name, int motor, uint64_t id, bool victor=false) ;

            /// \brief destroy the subsystem, freeing up the motor controllers
            virtual ~SingleMotorSubsystem();

            virtual bool canAcceptAction(ActionPtr action) ;

            /// \brief returns true if the motor is running
            /// \returns true if the motor is running
            bool isRunning() const {
                return current_power_ > epsilon ;
            }

        protected:
            /// \brief set the power (PWM) percentage for the motor.
            /// \param power the power to apply to the motor, between -1.0 and 1.0
            void setMotor(double power) {
                motor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, power) ;
                current_power_ = power ;
            }

        private:
            // The TalonSRX motor controller is a talon is used
            std::shared_ptr<IMotorController> motor_;

            // The current power applied to the motor (-1 to 1)
            double current_power_ ;

            // The motor index
            int index_ ;

            // The message id
            uint64_t msg_id_ ;
        }  ;
    }
}
