#include "PhaserAutoModeBase.h"
#include "Phaser.h"
#include "phasercameratracker/SetThresholdAction.h"
#include "carloshatch/CarlosHatchImpactAction.h"
#include <gamepiecemanipulator/WaitForHatch.h>
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

        void PhaserAutoModeBase::insertAutoModeLeg(const std::string &height, const std::string &angle, const std::string &path, bool rear, 
                                bool hashatch, double visiondelay, double turndelay, double detect)
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto db = phaser.getPhaserRobotSubsystem()->getTankDrive() ;
            auto game = phaser.getPhaserRobotSubsystem()->getGameManipulator() ;           
            auto vision = phaser.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto hatchholder = phaser.getPhaserRobotSubsystem()->getGameManipulator()->getHatchHolder() ;
            std::shared_ptr<xero::base::LightSensorSubsystem> lines ;

            ActionPtr act ;
            std::shared_ptr<ParallelAction> parallel ;
            std::shared_ptr<TerminateAction> term ;
            ActionSequencePtr seq ;

            const char *power ;
            const char *dist ;
            const char *adjust ;

            if (rear) {
                power = "linefollower:back:power" ;
                dist = "linefollower:back:distance" ;
                adjust = "linefollower:back:adjust" ;
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
            // Start the initial path
            //
            ////////////////////////////////////////////////////            

            //
            // Parallel action, performs path following and turntable/lift change in parallel
            //
            parallel = std::make_shared<ParallelAction>() ;

            //
            // The first action in the parallel sets the vision detection threshold
            //
            act = std::make_shared<SetThresholdAction>(*vision, detect) ;
            parallel->addSubActionPair(vision, act) ;

            //
            // The second entry in the parallel entry is a sequence
            //
            seq = std::make_shared<ActionSequence>(phaser.getMessageLogger()) ;

            //
            // Add a delay to ensure the robot has moved away from the previous
            // feature before we start turning the turntable.  This might be a rocks,
            // cargo ship, or loading station.
            //
            act = std::make_shared<DelayAction>(turndelay) ;
            seq->pushAction(act) ;

            //
            // Move the turntable to the right spot
            //
            act = std::make_shared<ReadyAction>(*game, height, angle, true) ;
            seq->pushSubActionPair(game, act, true) ;

            //
            // And stick out our arm and ready it for a collect or place action
            //
            act = std::make_shared<CarlosHatchImpactAction>(*hatchholder) ;
            seq->pushSubActionPair(hatchholder, act) ;

            //
            // Add this sequence as the second entry in the parallel
            //
            parallel->addAction(seq) ;

            //
            // The third entry in the parallel entry is a sequence as well
            //
            seq = std::make_shared<ActionSequence>(phaser.getMessageLogger()) ;            
            
            //
            // Create the sequence that follows the path, switches to vision, switches
            // to line following
            //
            act = std::make_shared<TankDriveFollowPathAction>(*db, path, rear) ;
        
            //
            // This is a terminatable action that follows a path and terminates
            // when vision picks up the targets.  The visiondelay ensures we get away
            // from the previous target before we start looking for a new vision target.
            //
            // A terminatable action is one that may be canceled by another object that
            // detects specific conditions.  In this case it is vision detecting a vision
            // target.
            //
            term = std::make_shared<TerminateAction>(db, act, phaser, visiondelay) ;
            term->addTerminator(vision) ;
            seq->pushAction(term) ;

            //
            // This is a terminatable sequence that drives via vision and terminates
            // when the line follower picks up the line.
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
            // Add the sequence to the parallel
            //
            parallel->addAction(seq) ;

            //
            // And push the entire parallel sequence onto this automode
            //
            pushAction(parallel) ;            
        }
    }
}