#pragma once

#include "swerverobotsubsystem/SwerveRobotSubsystem.h"
#include <Robot.h>


namespace xero {
    namespace swerve {

        /// \brief the concrete class that represents the robot Phoenix
        class SwerveRobot : public xero::base::Robot {
        public:
            SwerveRobot() ;
            virtual ~SwerveRobot() ;

            /// \brief return the robot subsystem cast to the phoenix specific type
            /// \returns the robot subsystem cast to the phoenix specific type
            std::shared_ptr<SwerveRobotSubsystem> getSwerveRobotSubsystem() {
                return std::dynamic_pointer_cast<SwerveRobotSubsystem>(getRobotSubsystem()) ;
            }

            virtual void loadPaths() ;

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

            virtual bool isCompBot() ;

        private:
        } ;
    }
}
