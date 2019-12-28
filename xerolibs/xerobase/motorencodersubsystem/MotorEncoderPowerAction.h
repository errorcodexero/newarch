#pragma once

#include <singlemotorsubsystem/SingleMotorPowerAction.h>

namespace xero
{
    namespace base
    {
        class MotorEncoderSubsystem ;
        
        class MotorEncoderPowerAction : public SingleMotorPowerAction
        {
        public:
            /// \brief Create an action that sets the motor to a given power.
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
            /// \param subsystem the subsystem this action is applied to
            /// \param power the power to apply to the motor (-1 to 1)
            MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, double power) ;

            /// \brief Create an action that sets the motor to a given power.
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
            /// \param subsystem the subsystem this action is applied to
            /// \param name the name of a setting in the settings file that contains the power for the motor
            MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, const std::string &name) ;

            /// \brief Create an action that sets the motor to a given power for a fixed duration of time
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor for the duration given.  The action is done when the duration is complete.  When
            /// the duration is complete, the motor is set back to zero power.
            /// \param subsystem the subsystem this action is applied to
            /// \param power the power to apply to the motor (-1 to 1)
            /// \param duration the amount of time to apply the given power
            MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, double power, double duration) ;

            /// \brief Create an action that sets the motor to a given power for a fixed duration of time
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor for the duration given.  The action is done when the duration is complete.  When
            /// the duration is complete, the motor is set back to zero power.
            /// \param subsystem the subsystem this action is applied to
            /// \param name the name of a setting in the settings file that contains the power for the motor
            /// \param durname the name of the setting in the settings file that contains the duration of the power
            MotorEncoderPowerAction(MotorEncoderSubsystem &subsystem, const std::string &name, const std::string &durname) ;

            virtual ~MotorEncoderPowerAction();

            virtual void run();
            virtual void start();
            virtual bool isDone();


        private:
            int plot_id_;
            static std::vector<std::string> columns_;
        };
    }
}