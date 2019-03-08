#include "automodes/TankDriveScrubMode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveScrubCharAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        TankDriveScrubMode::TankDriveScrubMode(Robot &robot, double power, double duration, bool highgear) : PhaserAutoModeBase(robot, "RotateChar", "Rotate Scrub")
        {            
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            pushSubActionPair(db, std::make_shared<TankDriveScrubCharAction>(*db, 1.0, 0.0, 0.3, highgear)) ;
        }

        TankDriveScrubMode::~TankDriveScrubMode()
        {            
        }
    }
}