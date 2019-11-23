#include "RanseurRobotSubsystem.h"
#include "ranseurids.h"
#include "tubcollector/TubCollector.h"
#include "tubarm/TubArm.h"
#include "tubwrist/TubWrist.h"
#include <Robot.h>
#include <list>

#include <actions/Action.h>
#include "Subsystem.h"


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
            oi_ = std::make_shared<RanseurOISubsystem>(this) ;
            addChild(oi_) ;                    

            tracker_ = std::make_shared<RanseurCameraTracker>(this) ;
            tracker_->setCameraMode(CameraTracker::CameraMode::DriverViewing) ;               
            tracker_->setCameraIndex(0) ;
            addChild(tracker_) ;

            bunnyArm_ = std::make_shared<BunnyArm>(this);
            addChild(bunnyArm_);
       
            manipulator_ = std::make_shared<TubManipulatorSubsystem>(this) ;
            addChild(manipulator_) ;

            for(int i = 0 ; i < 10 ; i++)
            {
                auto dout = std::make_shared<frc::DigitalOutput>(i) ;
                dout->Set(false) ;
                dios_.push_back(dout) ;
            }
        }

        RanseurRobotSubsystem::~RanseurRobotSubsystem() {
        }

        bool RanseurRobotSubsystem::canAcceptAction(ActionPtr act) {
            return false;
        }
    }
}
