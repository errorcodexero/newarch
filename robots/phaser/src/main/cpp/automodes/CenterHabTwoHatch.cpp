#include "automodes/CenterHabTwoHatch.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/LineFollowAction.h>
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <gamepiecemanipulator/ScoreHatch.h>
#include <gamepiecemanipulator/ReadyAction.h>
#include <gamepiecemanipulator/CompleteLSHatchCollect.h>
#include <lightsensor/LightSensorSubsystem.h>
#include <phasercameratracker/PhaserCameraTracker.h>
#include <phasercameratracker/DriveByVisionAction.h>
#include <cameratracker/CameraChangeAction.h>
#include <TerminateAction.h>
#include <ParallelAction.h>
#include <DelayAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabTwoHatch::CenterHabTwoHatch(Robot &robot, bool left) : PhaserAutoModeBase(robot, "CenterHabTwoHatch", "Start in center, score two on cargo ship, getting extra hatch from left loading station")
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto db = phaser.getPhaserRobotSubsystem()->getTankDrive() ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto frontline = phaser.getPhaserRobotSubsystem()->getFrontLineSensor() ;
            auto rearline = phaser.getPhaserRobotSubsystem()->getBackLineSensor() ;            
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            ActionPtr act ;
            std::shared_ptr<ParallelAction> parallel ;
            std::shared_ptr<TerminateAction> term ;
            ActionPtr dispatch ;
            ActionSequencePtr seq ;
            
            //
            // Setup the camera for auto mode
            //
            act = std::make_shared<CameraChangeAction>(*vision, 0, CameraTracker::CameraMode::TargetTracking) ;
            pushSubActionPair(vision, act) ;

            //
            // Go score the first hatch on the cargo ship
            //
            parallel = std::make_shared<ParallelAction>() ;

            if (left)
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CenterHab2CargoFrontLeft", "straight") ;
            else
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CenterHab2CargoFrontRight", "straight") ;            
                
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
            term->addTerminator(frontline) ;
            pushAction(term) ;

            const char *power = "linefollower:front:power" ;
            const char *dist = "linefollower:front:distance" ;
            const char *adjust = "linefollower:front:adjust" ;
            act = std::make_shared<LineFollowAction>(*frontline, *db, power, dist, adjust) ;
            pushSubActionPair(db, act) ;

            act = std::make_shared<ScoreHatch>(*game) ;

#ifdef NOTYET
            //
            // Back to the loading station
            //
            pushSubActionPair(game, act) ;
            parallel = std::make_shared<ParallelAction>() ;

            if (left)
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CargoFrontLeftLSLeft", "curve1") ;
            else {
                assert(false) ;
            }
                
            dispatch = std::make_shared<DispatchAction>(db, act, true) ;
            parallel->addAction(dispatch) ;

            seq = std::make_shared<ActionSequence>(phaser.getMessageLogger()) ;
            seq->pushAction(std::make_shared<DelayAction>(3.0)) ;
            
            angle = "turntable:angle:hatch:collect:south" ;
            height = "lifter:height:hatch:collect:south" ;
            act = std::make_shared<ReadyAction>(*game, height, angle) ;
            dispatch = std::make_shared<DispatchAction>(game, act, true) ;
            seq->pushAction(dispatch) ;

            parallel->addAction(seq) ;

            term = std::make_shared<TerminateAction>(parallel, phaser.getMessageLogger()) ;
            term->addTerminator(vision) ;
            pushAction(term) ;

            act = std::make_shared<DriveByVisionAction>(*db, *vision) ;

            term = std::make_shared<TerminateAction>(act, phaser.getMessageLogger()) ;
            term->addTerminator(rearline) ;
            pushAction(term) ;

            power = "linefollower:front:power" ;
            dist = "linefollower:front:distance" ;
            adjust = "linefollower:front:adjust" ;
            act = std::make_shared<LineFollowAction>(*rearline, *db, power, dist, adjust) ;
            pushSubActionPair(db, act) ;

            act = std::make_shared<CompleteLSHatchCollect>(*game) ;            
            pushSubActionPair(game, act) ;
#endif
        }

        CenterHabTwoHatch::~CenterHabTwoHatch()
        {            
        }    
    }
}