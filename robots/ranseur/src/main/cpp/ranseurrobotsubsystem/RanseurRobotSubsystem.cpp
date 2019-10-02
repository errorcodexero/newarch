#include "RanseurRobotSubsystem.h"
#include "ranseurids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        RanseurRobotSubsystem::RanseurRobotSubsystem(Robot &robot) : RobotSubsystem(robot, "ranseur") {
            //
            // Add the tank drive.  This is handled by the base class RobotSubsystem since all robots have a drivebase
            // and for now they are all tank drives
            //
            addTankDrive() ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<RanseurOISubsystem>(robot) ;
            addChild(oi_) ;                    

            tracker_ = std::make_shared<RanseurCameraTracker>(robot) ;
            tracker_->setCameraMode(CameraTracker::CameraMode::DriverViewing) ;               
            tracker_->setCameraIndex(0) ;
            addChild(tracker_) ;
        }

        RanseurRobotSubsystem::~RanseurRobotSubsystem() {
        }

        bool RanseurRobotSubsystem::canAcceptAction(ActionPtr act) {
            return false;
        }
    }
}

