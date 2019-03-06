#include "PhaserAutoModeBase.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/LineFollowAction.h>
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <gamepiecemanipulator/ReadyAction.h>
#include <lightsensor/LightSensorSubsystem.h>
#include <phasercameratracker/DriveByVisionAction.h>
#include <cameratracker/CameraChangeAction.h>
#include <TerminateAction.h>
#include <ParallelAction.h>
#include <DelayAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserAutoModeBase::PhaserAutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        PhaserAutoModeBase::~PhaserAutoModeBase() 
        {            
        }

        void PhaserAutoModeBase::insertAutoModeLeg(const std::string &height, const std::string &angle, const std::string &path, bool rear, ActionPtr finish)
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto db = phaser.getPhaserRobotSubsystem()->getTankDrive() ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;           
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            std::shared_ptr<xero::base::LightSensorSubsystem> lines ;

            ActionPtr act ;
            std::shared_ptr<ParallelAction> parallel ;
            std::shared_ptr<TerminateAction> term ;
            ActionSequencePtr seq ;

            const char *power ;
            const char *dist ;
            const char *adjust ;

            if (rear) {
                power = "linefollower:front:power" ;
                dist = "linefollower:front:distance" ;
                adjust = "linefollower:front:adjust" ;
                lines = phaser.getPhaserRobotSubsystem()->getBackLineSensor() ;                     
            }
            else
            {
                power = "linefollower:front:power" ;
                dist = "linefollower:front:distance" ;
                adjust = "linefollower:front:adjust" ;
                lines = phaser.getPhaserRobotSubsystem()->getFrontLineSensor() ;                
            }
            

            ////////////////////////////////////////////////////
            //
            // Start the initial hatch
            //
            ////////////////////////////////////////////////////            

            //
            // Parallel action, performs path following and turntable/lift change in parallel
            //
            parallel = std::make_shared<ParallelAction>() ;

            //
            // Move the turntable to the right spot
            //
            act = std::make_shared<ReadyAction>(*game, height, angle) ;
            parallel->addSubActionPair(game, act, true) ;

            //
            // Create the sequence that follows the path, switches to vision, switches
            // to line following
            //
            seq = std::make_shared<ActionSequence>(phaser.getMessageLogger()) ;
            act = std::make_shared<TankDriveFollowPathAction>(*db, path) ;
        
            //
            // This is a terminatable sequence that follows a path and terminates
            // when vision picks up the targets
            //
            term = std::make_shared<TerminateAction>(db, act, phaser) ;
            term->addTerminator(vision) ;
            seq->pushAction(term) ;

            //
            // This is a terminatable sequence that drives via vision and terminates
            // when the line follower picks up the line
            //
            act = std::make_shared<DriveByVisionAction>(*db, *vision, rear) ;
            term = std::make_shared<TerminateAction>(db, act, phaser) ;
            term->addTerminator(lines) ;
            seq->pushAction(term) ;

            //
            // This is the line following action
            //
            act = std::make_shared<LineFollowAction>(*lines, *db, power, dist, adjust) ;
            seq->pushSubActionPair(db, act) ;

            //
            // This is the finish action
            //
            seq->pushSubActionPair(game, finish) ;

            //
            // Add the sequence to the parallel
            //
            parallel->addAction(seq) ;

            //
            // And push the entire mess onto the automode
            //
            pushAction(parallel) ;            
        }
    }
}