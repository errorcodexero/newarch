#include "automodes/CenterHabHatchCargo.h"
#include "Phaser.h"
#include <phasercameratracker/PhaserCameraTracker.h>
#include <cameratracker/CameraChangeAction.h>
#include "gamepiecemanipulator/ScoreHatch.h"
#include "gamepiecemanipulator/CompleteLSHatchCollect.h"
#include <tankdrive/TankDriveFollowPathAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabHatchCargo::CenterHabHatchCargo(Robot &robot, bool left, const char *name, const char *desc) : PhaserAutoModeBase(robot, name, desc)
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto db = phaser.getPhaserRobotSubsystem()->getTankDrive() ;
      
            ActionPtr act ;
            
            //
            // Setup the camera for auto mode
            //
            act = std::make_shared<CameraChangeAction>(*vision, 0, CameraTracker::CameraMode::TargetTracking) ;
            pushSubActionPair(vision, act) ;

            //
            // Place first hatch
            //
            const char *angle = "turntable:angle:hatch:place:north" ;
            const char *height = "lifter:height:hatch:place:north:1" ;
            std::string path ;

            if (left)
                path = "CenterHab2CargoFrontLeft" ;
            else            
                path = "CenterHab2CargoFrontRight" ;

            act = std::make_shared<ScoreHatch>(*game) ;            
            insertAutoModeLeg(height, angle, path, false, 2.0, act) ;

            if (left)
                path = "CargoFrontLeftBalls" ;
            else
                path = "CargoFrontRightBalls" ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, path, true) ;
            pushSubActionPair(db, act) ;

            act = std::make_shared<FloorCollectCargoAction>(*game) ;
            pushSubActionPair(game, act) ;
        }

        CenterHabHatchCargo::~CenterHabHatchCargo()
        {            
        }    
    }
}