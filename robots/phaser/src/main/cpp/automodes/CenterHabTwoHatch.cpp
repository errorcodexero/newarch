#include "automodes/CenterHabTwoHatch.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/LineFollowAction.h>
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <gamepiecemanipulator/ScoreHatch.h>
#include <gamepiecemanipulator/ReadyAction.h>
#include <lightsensor/LightSensorSubsystem.h>
#include <phasercameratracker/PhaserCameraTracker.h>
#include <phasercameratracker/DriveByVisionAction.h>
#include <cameratracker/CameraChangeAction.h>
#include <TerminateAction.h>
#include <ParallelAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabTwoHatch::CenterHabTwoHatch(Robot &robot, bool left) : PhaserAutoModeBase(robot, "CenterHabTwoHatch", "Start in center, score two on cargo ship, getting extra hatch from left loading station")
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto db = phaser.getPhaserRobotSubsystem()->getTankDrive() ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto linefollow = phaser.getPhaserRobotSubsystem()->getFrontLineSensor() ;
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            ActionPtr act ;
            std::shared_ptr<ParallelAction> parallel ;
            std::shared_ptr<TerminateAction> term ;
            ActionPtr dispatch ;
            
            act = std::make_shared<CameraChangeAction>(*vision, 0, CameraTracker::CameraMode::TargetTracking) ;
            pushSubActionPair(vision, act) ;

            parallel = std::make_shared<ParallelAction>() ;

            if (left)
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CenterHab2CargoFrontLeft") ;
            else
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CenterHab2CargoFrontRight") ;            
                
            dispatch = std::make_shared<DispatchAction>(db, act, true) ;
            parallel->addAction(dispatch) ;
            
            const char *angle = "turntable:angle:hatch:place:north" ;
            const char *height = "lifter:height:hatch:place:north:1" ;
            act = std::make_shared<ReadyAction>(*game, height, angle) ;
            dispatch = std::make_shared<DispatchAction>(game, act, true) ;
            parallel->addAction(dispatch) ;

            term = std::make_shared<TerminateAction>(parallel, phaser.getMessageLogger()) ;
            term->addTerminator(vision) ;
            pushAction(term) ;

            act = std::make_shared<DriveByVisionAction>(*db, *vision) ;

            term = std::make_shared<TerminateAction>(act, phaser.getMessageLogger()) ;
            term->addTerminator(linefollow) ;
            pushAction(term) ;

            const char *power = "linefollower:front:power" ;
            const char *dist = "linefollower:front:distance" ;
            const char *adjust = "linefollower:front:adjust" ;
            act = std::make_shared<LineFollowAction>(*linefollow, *db, power, dist, adjust) ;
            pushSubActionPair(db, act) ;

            act = std::make_shared<ScoreHatch>(*game) ;
            pushSubActionPair(game, act) ;
        }

        CenterHabTwoHatch::~CenterHabTwoHatch()
        {            
        }    
    }
}