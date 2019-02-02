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

#ifdef SIMULATOR
            hatch_holder_ = std::make_shared<HatchHolder>(robot) ;
            addChild(hatch_holder_) ;

            lifter_ = std::make_shared<Lifter>(robot, MGR_GROUP_PHASER_LIFTER) ;
            addChild(lifter_) ;

            turntable_ = std::make_shared<Turntable>(robot, MGR_GROUP_PHASER_TURNTABLE) ;
            addChild(turntable_) ;
#endif

            tracker_ = std::make_shared<CameraTracker>(robot) ;\
            tracker_->setCameraIndex(0) ;
            addChild(tracker_) ;
        }

        PhaserRobotSubsystem::~PhaserRobotSubsystem() {
        }
    }
}
