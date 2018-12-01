#pragma once

#include "Action.h"
#include "basegroups.h"
#include "SingleMotorSubsystem.h"
#include "SingleMotorSubsystemAction.h"

namespace xero {
    namespace base {
		/// \brief This action assigns a power to the motor in the subsystem
		/// The power can be turned on and left on continously, or it can be turned on for a fixed
		/// duration of time.
		/// \sa xero::misc::SettingsParser
        class SingleMotorVoltageAction : public SingleMotorSubsystemAction {
        public:
			/// \brief Create an action that sets the motor to a given power.
			/// The power is applied immediately when the action is stared and remains applied
			/// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
			/// \param subsystem the subsystem this action is applied to
			/// \param power the power to apply to the motor (-1 to 1)
            SingleMotorVoltageAction(SingleMotorSubsystem &subsystem, double power) ;

			/// \brief Create an action that sets the motor to a given power.
			/// The power is applied immediately when the action is stared and remains applied
			/// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
			/// \param subsystem the subsystem this action is applied to
			/// \param name the name of a setting in the settings file that contains the power for the motor
            SingleMotorVoltageAction(SingleMotorSubsystem &subsystem, const std::string &name) ;

			/// \brief Create an action that sets the motor to a given power for a fixed duration of time
			/// The power is applied immediately when the action is stared and remains applied
			/// to the motor for the duration given.  The action is done when the duration is complete.  When
			/// the duration is complete, the motor is set back to zero power.
			/// \param subsystem the subsystem this action is applied to
			/// \param power the power to apply to the motor (-1 to 1)
			/// \param duration the amount of time to apply the given power
            SingleMotorVoltageAction(SingleMotorSubsystem &subsystem, double duty, double duration) ;

			/// \brief Create an action that sets the motor to a given power for a fixed duration of time
			/// The power is applied immediately when the action is stared and remains applied
			/// to the motor for the duration given.  The action is done when the duration is complete.  When
			/// the duration is complete, the motor is set back to zero power.
			/// \param subsystem the subsystem this action is applied to
			/// \param name the name of a setting in the settings file that contains the power for the motor
			/// \param durname the name of the setting in the settings file that contains the duration of the power
            SingleMotorVoltageAction(SingleMotorSubsystem &subsystem, const std::string &name, const std::string &durname) ;			

			/// \brief destroy the action object
            virtual ~SingleMotorVoltageAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
			/// \brief Returns true if the action is complete.
			/// If the power has no duration, the power is assigned and this method returns true immediately.  If the power
			/// has a duration, this method only returns true after the duration has expired and the motor power is set back
			/// to zero.
			/// \returns true if the action is complete
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
            double duty_cycle_;
			bool timed_ ;
			double duration_ ;
			bool is_done_ ;
			double start_ ;
        };
    }
}