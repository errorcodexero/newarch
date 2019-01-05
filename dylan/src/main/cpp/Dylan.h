#pragma once

#include <tankdrive/TankDrive.h>
#include <Robot.h>

namespace xero {
    namespace dylan {

        /// \brief the concrete class that represents the robot Phoenix
        class Dylan : public xero::base::Robot {
        public:
            Dylan() ;

#ifdef NOTYET
            /// \brief return the robot subsystem cast to the phoenix specific type
            /// \returns the robot subsystem cast to the phoenix specific type
            std::shared_ptr<DylanRobotSubsystem> getDylanRobotSubsystem() {
                return std::dynamic_pointer_cast<DylanRobotSubsystem>(getRobotSubsystem()) ;
            }
#endif

        protected:

            /// \brief called to initialize the robot, basically creating the subsystems
            virtual void RobotHardwareInit() ;

            /// \brief called to create the autonomous controller
            /// \returns a robot controller
            virtual std::shared_ptr<xero::base::ControllerBase> createAutoController() ;
            
            /// \brief called to create the teleop controller
            /// \returns a robot controller
            virtual std::shared_ptr<xero::base::ControllerBase> createTeleopController() ;

            /// \brief called to create the test controller
            /// \returns a robot controller
            virtual std::shared_ptr<xero::base::ControllerBase> createTestController() ;

            /// \brief enable specific message for logging
            virtual void enableSpecificMessages() ; 
        } ;
    }
}
