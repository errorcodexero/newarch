#include "automodes/StraightCharAutoMode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveCharAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        StraightCharAutoMode::StraightCharAutoMode(Robot &robot, double power, double duration, bool highgear) : PhaserAutoModeBase(robot, "StraightChar", "Characterize the drive base")
        {            
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            pushSubActionPair(db, std::make_shared<TankDriveCharAction>(*db, duration, power, highgear)) ;
        }

        StraightCharAutoMode::~StraightCharAutoMode()
        {            
        }
    }
}