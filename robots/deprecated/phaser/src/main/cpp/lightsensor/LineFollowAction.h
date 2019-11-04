#pragma once

#include "Action.h"
#include "basegroups.h"
#include "lightsensor/LightSensorSubsystem.h"
#include <tankdrive/actions/TankDriveAction.h>
#include <tankdrive/TankDrive.h>


/// \file


namespace xero {
    namespace phaser {
        /// \brief This action assigns a power to the motor in the subsystem
        /// The power can be turned on and left on continously, or it can be turned on for a fixed
        /// duration of time.
        /// \sa xero::misc::SettingsParser
        class LineFollowAction : public xero::base::TankDriveAction {
        public:
            /// \brief Create an action that sets the motor to a given power.
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
            /// \param subsystem the subsystem this action is applied to
            /// \param power the power to apply to the motor (-1 to 1)
            LineFollowAction(LightSensorSubsystem &ls_subsystem, xero::base::TankDrive &db_subsystem, double power, double distance, double power_adjust) ;

            /// \brief Create an action that sets the motor to a given power.
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
            /// \param subsystem the subsystem this action is applied to
            /// \param name the name of a setting in the settings file that contains the power for the motor
            LineFollowAction(LightSensorSubsystem &ls_subsystem, xero::base::TankDrive &db_subsystem, const std::string &power_name, const std::string &distance_name, const std::string &power_adjust_name) ;

            /// \brief destroy the action object
            virtual ~LineFollowAction() ;

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
            LightSensorSubsystem &ls_subsystem_ ;
            double distance_ ;
            double power_;
            double power_adjust_;
            double start_distance_;
            double stalled_threshold_ ;
            bool is_done_ ;
            
            std::list<double> distances_ ;
        };
    }
}