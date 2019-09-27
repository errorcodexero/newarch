#include "SwerveSubsystem.h"
#include "swerveids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace swerve {
        SwerveSubsystem::SwerveSubsystem(Robot &robot) : RobotSubsystem(robot, "swerve") {
            //
            // Add the tank drive.  This is handled by the base class RobotSubsystem since all robots have a drivebase
            // and for now they are all tank drives
            //
            addTankDrive() ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<SwerveOISubsystem>(robot) ;
            addChild(oi_) ;

            motor1_ = std::make_shared<SingleMotorSubsystem>(robot, "motor1", "hw:motor1:id", 0x100000000) ;
            addChild(motor1_) ;

            motor2_ = std::make_shared<SingleMotorSubsystem>(robot, "motor2", "hw:motor2:id", 0x200000000) ;
            addChild(motor2_) ;
        }

        SwerveSubsystem::~SwerveSubsystem() {
        }

        bool SwerveSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
