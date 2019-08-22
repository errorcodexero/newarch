#include "SwerveRobotSubsystem.h"
#include "swerveids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace swerve {
        SwerveRobotSubsystem::SwerveRobotSubsystem(Robot &robot) : RobotSubsystem(robot, "phaser") {

            //
            // Add the drivebase to the robot
            //
            db_ = std::make_shared<SwerveDriveBase>(robot, 0, 1, 2, 3, 4, 5, 6, 7) ;
            addChild(db_) ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<SwerveOISubsystem>(robot) ;
            addChild(oi_) ;        
        }

        SwerveRobotSubsystem::~SwerveRobotSubsystem() {
        }

        bool SwerveRobotSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}

