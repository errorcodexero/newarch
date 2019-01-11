#pragma once

#include "wings/Wings.h"
#include "intake/Intake.h"
#include "phlifter/Lifter.h"
#include "grabber/Grabber.h"
#include "collector/Collector.h"
#include "phoenixsubsystem/PhoenixRobotSubsystem.h"
#include "phoenixoi/PhoenixOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {

        /// \brief the concrete class that represents the robot Phoenix
        class Phoenix : public xero::base::Robot {
        public:
            Phoenix() ;

            /// \brief return the robot subsystem cast to the phoenix specific type
            /// \returns the robot subsystem cast to the phoenix specific type
            std::shared_ptr<PhoenixRobotSubsystem> getPhoenixRobotSubsystem() {
                return std::dynamic_pointer_cast<PhoenixRobotSubsystem>(getRobotSubsystem()) ;
            }

        protected:

            /// \brief called to initialize the robot, basically creating the subsystems
            virtual void RobotHardwareInit() ;

            /// \brief read the paths required for the robot
            virtual void readPaths() ;

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
