#pragma once

#include "phaseroi/PhaserOISubsystem.h"
#include "linefollower/LineFollower.h"
#include <tankdrive/TankDrive.h>
#include <lifter/Lifter.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace phaser {

        class PhaserRobotSubsystem : public xero::base::RobotSubsystem {
        public:
            PhaserRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~PhaserRobotSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<PhaserOISubsystem> getOI() {
                return oi_ ;
            }    
        private:
            std::shared_ptr<PhaserOISubsystem> oi_ ;
            std::shared_ptr<LineFollower> follower_ ;
        } ;
    }
}