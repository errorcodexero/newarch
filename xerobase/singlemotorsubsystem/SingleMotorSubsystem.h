#pragma once

#include "Subsystem.h" 
#include "SingleMotorSubsystemAction.h" 
#include "XeroTalonSRX.h"
#include <VictorSP.h>


typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
    namespace base {

		/// \brief This class is a subsystem that consists of a single motor.
		/// The motor can be a controlled via a VictorSP motor controller or
		/// via a TalonSRX motor controller.<br><br>
		/// <b>Actions:</b><br>
		///     - SingleMotorVoltageAction
        class SingleMotorSubsystem : public Subsystem {
			friend class SingleMotorSubsystemAction;
			friend class SingleMotorVoltageAction ;
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
			/// \param victor if true use a VictorSP motor controller, otherwise create a TalonSRX
			/// \sa xero::misc::SettingsParser
            SingleMotorSubsystem(Robot &robot, const std::string &name, const std::string &motor, bool victor=false) ;

			/// \brief Create a new subsystem object
			/// The motor parameter is the number of the PWM to use for VictorSP or the CAN address
			/// of the controller for a TalonSRX. 
			/// \param robot a reference to the robot for this subsystem
			/// \param name the name of this subsystem, must be unique across all subsystems
			/// \param motor the actual number (PWM or CAN address) for the motor controller
			/// \param victor if true use a VictorSP motor controller, otherwise create a TalonSRX
			/// \sa xero::misc::SettingsParser
            SingleMotorSubsystem(Robot &robot, const std::string &name, int motor, bool victor=false) ;

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
            void setMotor(double power)
            {
				if (vmotor_ != nullptr)
					vmotor_->Set(power) ;
				else
					tmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, power) ;

				current_power_ = power ;
            }		

        private:
			// The victor motor controller if a victor is used
            std::shared_ptr<frc::VictorSP> vmotor_;

			// The TalonSRX motor controller is a talon is used
            std::shared_ptr<TalonSRX> tmotor_;

			// The current power applied to the motor (-1 to 1)
			double current_power_ ;
        }  ;
    }
}
