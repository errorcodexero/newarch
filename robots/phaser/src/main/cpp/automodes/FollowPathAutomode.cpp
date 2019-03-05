#include "automodes/FollowPathAutomode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveFollowPathAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        FollowPathAutomode::FollowPathAutomode(Robot &robot, const std::string &name, bool reverse) : PhaserAutoModeBase(robot, "Followpath  - " + name, "Follow a path")
        {            
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            pushSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, name, reverse)) ;
        }

        FollowPathAutomode::~FollowPathAutomode()
        {            
        }
    }
}