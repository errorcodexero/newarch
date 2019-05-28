#include <tankdrive/FollowPathAutomode.h>
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveFollowPathAction.h>
#include <cassert>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace base {
        FollowPathAutomode::FollowPathAutomode(Robot &robot, const std::string &name, bool reverse) : AutoMode(robot, "Followpath  - " + name, "Follow a path")
        {            
            auto db = robot.getDriveBase() ;
            std::shared_ptr<TankDrive> tank = std::dynamic_pointer_cast<TankDrive>(db) ;
            assert(tank != nullptr) ;

            pushSubActionPair(db, std::make_shared<TankDriveFollowPathAction>(*tank, name, reverse)) ;
        }

        FollowPathAutomode::FollowPathAutomode(Robot &robot, std::vector<std::string> &paths, std::vector<bool> &reverses) : AutoMode(robot, "Followpath - multiple", "Follow a path")
        {            
            auto db = robot.getDriveBase() ;
            std::shared_ptr<TankDrive> tank = std::dynamic_pointer_cast<TankDrive>(db) ;
            assert(tank != nullptr) ;

            assert(paths.size() == reverses.size()) ;
            for(size_t i = 0 ; i < paths.size() ; i++)
                pushSubActionPair(tank, std::make_shared<TankDriveFollowPathAction>(*tank, paths[i], reverses[i])) ;
        }        

        FollowPathAutomode::~FollowPathAutomode()
        {            
        }
    }
}