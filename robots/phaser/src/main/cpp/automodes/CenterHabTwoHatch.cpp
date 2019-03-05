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

            ////////////////////////////////////////////////////
            //
            // Start the initial hatch
            //
            ////////////////////////////////////////////////////            

            //
            // Go score the first hatch on the cargo ship
            //
            parallel = std::make_shared<ParallelAction>() ;

            const char *angle = "turntable:angle:hatch:place:north" ;
            const char *height = "lifter:height:hatch:place:north:1" ;
            act = std::make_shared<ReadyAction>(*game, height, angle) ;
            dispatch = std::make_shared<DispatchAction>(game, act, true) ;
            parallel->addAction(dispatch) ;


            seq = std::make_shared<ActionSequence>(phaser.getMessageLogger()) ;

            if (left)
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CenterHab2CargoFrontLeft", "curve2") ;
            else
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CenterHab2CargoFrontRight", "curve2") ;            
                
        
            term = std::make_shared<TerminateAction>(db, act, phaser) ;
            term->addTerminator(vision) ;
            seq->pushAction(term) ;

            act = std::make_shared<DriveByVisionAction>(*db, *vision) ;
            term = std::make_shared<TerminateAction>(db, act, phaser) ;
            term->addTerminator(frontline) ;
            seq->pushAction(term) ;

            const char *power = "linefollower:front:power" ;
            const char *dist = "linefollower:front:distance" ;
            const char *adjust = "linefollower:front:adjust" ;
            act = std::make_shared<LineFollowAction>(*frontline, *db, power, dist, adjust) ;
            seq->pushSubActionPair(db, act) ;

            act = std::make_shared<ScoreHatch>(*game) ;
            seq->pushSubActionPair(game, act) ;

            parallel->addAction(seq) ;

            pushAction(parallel) ;

            ////////////////////////////////////////////////////
            //
            // Finihshed the initial hatch
            //
            ////////////////////////////////////////////////////


            ////////////////////////////////////////////////////
            //
            // Start the second hatch
            //
            ////////////////////////////////////////////////////            


            //
            // Go score the first hatch on the cargo ship
            //
            parallel = std::make_shared<ParallelAction>() ;

            angle = "turntable:angle:hatch:collect:south" ;
            height = "lifter:height:hatch:collect:south" ;
            act = std::make_shared<ReadyAction>(*game, height, angle) ;
            dispatch = std::make_shared<DispatchAction>(game, act, true) ;
            parallel->addAction(dispatch) ;

            seq = std::make_shared<ActionSequence>(phaser.getMessageLogger()) ;

            if (left)
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CargoFrontLeftLSLeft", "curve2") ;
            else
                act = std::make_shared<TankDriveFollowPathAction>(*db, "CargoFrontLeftLSRight", "curve2") ;
        
            term = std::make_shared<TerminateAction>(db, act, phaser) ;
            term->addTerminator(vision) ;
            seq->pushAction(term) ;

            act = std::make_shared<DriveByVisionAction>(*db, *vision, true) ;
            term = std::make_shared<TerminateAction>(db, act, phaser) ;
            term->addTerminator(rearline) ;
            seq->pushAction(term) ;

            power = "linefollower:back:power" ;
            dist = "linefollower:back:distance" ;
            adjust = "linefollower:back:adjust" ;
            act = std::make_shared<LineFollowAction>(*rearline, *db, power, dist, adjust) ;
            seq->pushSubActionPair(db, act) ;

            act = std::make_shared<CompleteLSHatchCollect>(*game) ;
            pushSubActionPair(game, act) ;

            parallel->addAction(seq) ;

            pushAction(parallel) ;

            ////////////////////////////////////////////////////
            //
            // Finihshed the second hatch
            //
            ////////////////////////////////////////////////////            
        }

        CenterHabTwoHatch::~CenterHabTwoHatch()
        {            
        }    
    }
}