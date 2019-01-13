#include "FollowPathAutoMode.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveCharAction.h>
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        FollowPathAutoMode::FollowPathAutoMode(Phoenix &robot) : PhoenixAutoMode(robot, "CrossTheLine") {
            create() ;
        }

        FollowPathAutoMode::~FollowPathAutoMode() {
        }

        void FollowPathAutoMode::create() {
            Phoenix &robot = getRobot() ;
            auto db = robot.getPhoenixRobotSubsystem()->getTankDrive() ;

            auto act = std::make_shared<TankDriveCharAction>(*db, 1.0, 0.2) ;
            addAction(db, act) ;
            
    //            followPath("TestPathOne") ;
        }
    }
}