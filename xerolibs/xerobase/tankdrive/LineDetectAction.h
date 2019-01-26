#pragma once

#include "Action.h"
#include "basegroups.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "TankDriveAction.h"


/// \file


namespace xero {
    namespace base {
        /// \brief This action assigns a power to the motor in the subsystem
        /// The power can be turned on and left on continously, or it can be turned on for a fixed
        /// duration of time.
        /// \sa xero::misc::SettingsParser
        class LineDetectAction : public TankDriveAction {
        public:
            /// \brief Create an action that sets the motor to a given power.
            /// The power is applied immediately when the action is stared and remains applied
            /// to the motor indefinitely.  This action is considered done (/sa isDone) immediately.
            /// \param subsystem the subsystem this action is applied to
            /// \param power the power to apply to the motor (-1 to 1)
            LineDetectAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem) ;

            /// \brief destroy the action object
            virtual ~LineDetectAction() ;

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
        bool is_done_ ;

        };
    }
}