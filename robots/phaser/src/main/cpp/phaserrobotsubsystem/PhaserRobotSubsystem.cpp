#include "PhaserRobotSubsystem.h"
#include "ClimbAction.h"
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

            front_line_sensor_ = std::make_shared<LightSensorSubsystem>(robot, "front", "hw:linesensor:front:", 3) ;    
            addChild(front_line_sensor_) ;

            back_line_sensor_ = std::make_shared<LightSensorSubsystem>(robot, "back", "hw:linesensor:back:", 3) ;    
            addChild(back_line_sensor_) ;            

            tracker_ = std::make_shared<PhaserCameraTracker>(robot) ;
            tracker_->setCameraMode(CameraTracker::CameraMode::DriverViewing) ;               
            tracker_->setCameraIndex(0) ;
            addChild(tracker_) ;

            game_piece_man_ = std::make_shared<GamePieceManipulator>(robot) ;
            addChild(game_piece_man_) ;

            climber_ = std::make_shared<Climber>(robot) ;
            addChild(climber_) ;
        }

        PhaserRobotSubsystem::~PhaserRobotSubsystem() {
        }

        bool PhaserRobotSubsystem::canAcceptAction(ActionPtr act) {
            std::shared_ptr<ClimbAction> climb = std::dynamic_pointer_cast<ClimbAction>(act) ;
            return climb != nullptr ;
        }
    }
}
