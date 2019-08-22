#include "SwerveRobotSubsystem.h"
#include "ClimbAction.h"
#include "StrafeAction.h"
#include "phaserids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        SwerveRobotSubsystem::SwerveRobotSubsystem(Robot &robot) : RobotSubsystem(robot, "phaser") {

            //
            // Add the drivebase to the robot
            //
            db_ = std::make_shared<SwerveDriveBase>(robot) ;
            addChild(db_) ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<PhaserOISubsystem>(robot) ;
            addChild(oi_) ;        
        }

        SwerveRobotSubsystem::~SwerveRobotSubsystem() {
        }

        bool SwerveRobotSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}

