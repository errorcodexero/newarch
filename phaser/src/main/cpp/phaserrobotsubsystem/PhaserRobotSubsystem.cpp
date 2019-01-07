#include "PhaserRobotSubsystem.h"
#include "phaserids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserRobotSubsystem::PhaserRobotSubsystem(Robot &robot) : RobotSubsystem(robot, "phaser") {
            auto &settings = robot.getSettingsParser() ;

            // Register message listener *before* any sub-system like the drive base which needs vision information.
            // The vision info received will only be present during 1 robot loop so ML's computeState()
            // must be called before other dependent sub-systems call their computeState() to access vision results.
            int socket_port_number = settings.getInteger("messagelistener:port") ;
            ml_ = std::make_shared<MessageListener>(robot, socket_port_number) ;
            addChild(ml_) ;

            //
            // Add the tank drive.  This is handled by the base class RobotSubsystem since all robots have a drivebase
            // and for now they are all tank drives
            //
            addTankDrive() ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<PhaserOISubsystem>(robot) ;
            addChild(oi_) ;            
        }

        PhaserRobotSubsystem::~PhaserRobotSubsystem() {
        }
    }
}
