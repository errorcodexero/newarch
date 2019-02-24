#pragma once

#include <phaserrobotsubsystem/PhaserRobotSubsystem.h>
#include <tankdrive/TankDrive.h>
#include <Robot.h>
#include <frc/Compressor.h>

namespace xero {
    namespace phaser {

        /// \brief the concrete class that represents the robot Phoenix
        class Phaser : public xero::base::Robot {
        public:
            Phaser() ;

            /// \brief return the robot subsystem cast to the phoenix specific type
            /// \returns the robot subsystem cast to the phoenix specific type
            std::shared_ptr<PhaserRobotSubsystem> getPhaserRobotSubsystem() {
                return std::dynamic_pointer_cast<PhaserRobotSubsystem>(getRobotSubsystem()) ;
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
            std::shared_ptr<frc::Compressor> compressor_ ;
            frc::DigitalInput *comp_bot_ ;
        } ;
    }
}
