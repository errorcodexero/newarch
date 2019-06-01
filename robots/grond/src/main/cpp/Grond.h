#pragma once

#include "grondsubsystem/GrondSubsystem.h"
#include <tankdrive/TankDrive.h>
#include <Robot.h>

namespace xero {
    namespace grond {

        /// \brief the concrete class that represents the robot Phoenix
        class Grond : public xero::base::Robot {
        public:
            Grond() ;

            /// \brief return the robot subsystem cast to the phoenix specific type
            /// \returns the robot subsystem cast to the phoenix specific type
            std::shared_ptr<GrondSubsystem> getGrondRobotSubsystem() {
                return std::dynamic_pointer_cast<GrondSubsystem>(getRobotSubsystem()) ;
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

            virtual bool isCompBot() {
                return true ;
            }
        } ;
    }
}
