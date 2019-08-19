#include "automodes/FollowPathAutomode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveFollowPathAction.h>
#include <cassert>

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

        FollowPathAutomode::FollowPathAutomode(Robot &robot, std::vector<std::string> &paths, std::vector<bool> &reverses) : PhaserAutoModeBase(robot, "Followpath - multiple", "Follow a path")
        {            
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            assert(paths.size() == reverses.size()) ;
            for(size_t i = 0 ; i < paths.size() ; i++)
                pushSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*db, paths[i], reverses[i])) ;
        }        

        FollowPathAutomode::~FollowPathAutomode()
        {            
        }
    }
}