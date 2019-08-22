#pragma once

#include "swerveoi/SwerveOISubsystem.h"
#include "swervesubsystem/SwerveDriveBase.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "phasercameratracker/PhaserCameraTracker.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "climber/Climber.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace phaser {

        //
        // Add gamemanipulator
        // Add climber
        //
        class SwerveRobotSubsystem : public xero::base::RobotSubsystem {
        public:
            SwerveRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~SwerveRobotSubsystem() ;

            std::shared_ptr<xero::swerve::SwerveDriveBase> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<SwerveOISubsystem> getOI() {
                return oi_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<SwerveOISubsystem> oi_ ;
            std::shared_ptr<SwerveDriveBase> db_ ;
        } ;
    }
}