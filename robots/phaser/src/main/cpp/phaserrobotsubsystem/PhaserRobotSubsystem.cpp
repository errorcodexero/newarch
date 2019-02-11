#include "PhaserRobotSubsystem.h"
#include "phaserids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserRobotSubsystem::PhaserRobotSubsystem(Robot &robot) : RobotSubsystem(robot, "phaser") {
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

            light_sensor_ = std::make_shared<LightSensorSubsystem>(robot, "linesensor", "hw:linesensor:", 3) ;    
            addChild(light_sensor_) ;

            hatch_holder_ = std::make_shared<HatchHolder>(robot) ;
            addChild(hatch_holder_) ;

            cargo_holder_ = std::make_shared<CargoHolder>(robot) ;
            addChild(cargo_holder_) ;

            cargo_intake_ = std::make_shared<CargoIntake>(robot, MSG_GROUP_CARGO_INTAKE) ;
            addChild(cargo_intake_) ;

            lifter_ = std::make_shared<Lifter>(robot, MSG_GROUP_PHASER_LIFTER) ;
            addChild(lifter_) ;

            turntable_ = std::make_shared<Turntable>(robot, MSG_GROUP_PHASER_TURNTABLE) ;
            addChild(turntable_) ;

            climber_ = std::make_shared<Climber>(robot) ;
            addChild(climber_) ;
            
            tracker_ = std::make_shared<PhaserCameraTracker>(robot) ;
            tracker_->setCameraIndex(0) ;
            tracker_->setCameraMode(CameraTracker::CameraMode::TargetTracking) ;            
            addChild(tracker_) ;
        }

        PhaserRobotSubsystem::~PhaserRobotSubsystem() {
        }
    }
}
