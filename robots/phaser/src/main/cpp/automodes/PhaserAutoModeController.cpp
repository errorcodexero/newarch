#include "PhaserAutoModeController.h"
#include "Phaser.h"
#include "hatchholder/HatchHolderAction.h"
#include "phasercameratracker/DriveByVisionAction.h"
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveScrubCharAction.h>
#include <tankdrive/LineDetectAction.h>
#include <tankdrive/LineFollowAction.h>
#include <tankdrive/TankDrivePowerAction.h>
#include <lifter/LifterCalibrateAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterHoldHeightAction.h>
#include <turntable/TurntableCalibrateAction.h>
#include <turntable/TurntableGoToAngleAction.h>
#include <turntable/TurnTableHoldAngleAction.h>
#include <turntable/TurntablePowerAction.h>
#include <DelayAction.h>
#include <TerminateAction.h>
#include <ParallelAction.h>
#include <frc/DriverStation.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserAutoModeController::PhaserAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        PhaserAutoModeController::~PhaserAutoModeController() {
        }

        void PhaserAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            ActionSequencePtr mode = nullptr ;

            
            getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
            getRobot().getMessageLogger() << "creating automode" ;
            getRobot().getMessageLogger().endMessage() ;

            switch(sel) {
            case 0:
                mode = createStraightCharAutoMode() ;
                break ;

            case 1:
                mode = createScrubCharAutoMode() ;
                break ;

            case 2:
                mode = createTestOne() ;
                break ;

            case 3:
                mode = createTestTwo() ;
                break ;

            case 4:
                mode = testHatchDeposit() ;
                break ;

            case 5:
                mode = testHatchHolder() ;
                break ;

            case 6:
                mode = testLifter() ;
                break ;

            case 7:
                mode = testLineFollower() ;
                break ;

            case 8:
                mode = testVision() ;
                break ;

            case 9:
                mode = testTurntable() ;
                break ;
            }
            setAction(mode) ;
        }

        
        ActionSequencePtr PhaserAutoModeController::testLifter() {
            std::string name = "Test Lifter" ;
            std::string desc = "Test the lifter go to height and hold" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            act = std::make_shared<LifterCalibrateAction>(*lifter) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<LifterGoToHeightAction>(*lifter, 45.0) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<LifterHoldHeightAction>(*lifter, 45.0) ;
            mode->pushSubActionPair(lifter, act, false) ;   

            act = std::make_shared<DelayAction>(10.0) ;
            mode->pushAction(act) ;  

            act = std::make_shared<LifterGoToHeightAction>(*lifter, 20.0) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<LifterHoldHeightAction>(*lifter, 20.0) ;
            mode->pushSubActionPair(lifter, act, false) ;

            return mode ;                
        }

        ActionSequencePtr PhaserAutoModeController::testTurntable() {
            std::string name = "Test Turntable" ;
            std::string desc = "Test the turntable" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<TurntableCalibrateAction>(*turntable) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, -20) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<DelayAction>(10.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, 180) ;
            mode->pushSubActionPair(turntable, act) ;    

            act = std::make_shared<DelayAction>(10.0) ;
            mode->pushAction(act) ;
            
            act = std::make_shared<TurntableGoToAngleAction>(*turntable, 0) ;
            mode->pushSubActionPair(turntable, act) ;                        

            return mode ;             
        }


        ActionSequencePtr PhaserAutoModeController::testHatchDeposit() {
            std::string name = "Test Deposit Hatch" ;
            std::string desc = "Test the deposit hatch sequence" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr childact, act, dispatch ;
            TermActionPtr termact ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;
            auto ls = phaserrobot->getLightSensor() ;
            auto cm = phaserrobot->getCameraTracker() ;

            childact = std::make_shared<TankDriveFollowPathAction>(*db, "CurveLeft") ;
            dispatch = std::make_shared<DispatchAction>(db, childact) ;
            termact = std::make_shared<TerminateAction>(dispatch, getRobot().getMessageLogger()) ;
            termact->addTerminator(cm) ;
            mode->pushAction(termact) ;

            childact = std::make_shared<DriveByVisionAction>(*db, *cm) ;
            dispatch = std::make_shared<DispatchAction>(db, childact) ;    

            mode->pushAction(act) ;

            act = std::make_shared<LineFollowAction>(*ls, *db, "linefollow:power", "linefollow:distance", "linefollow:adjust") ;
            mode->pushSubActionPair(db, act) ;            
            return mode ;         
        }        

        ActionSequencePtr PhaserAutoModeController::testVision() {
            std::string name = "Test Vision" ;
            std::string desc = "Test the vision system" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr childact, act, dispatch ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;
            auto cm = phaserrobot->getCameraTracker() ;

            act = std::make_shared<DriveByVisionAction>(*db, *cm) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;
        }         

        ActionSequencePtr PhaserAutoModeController::testLineFollower() {
            std::string name = "Test Line Follower" ;
            std::string desc = "Test the line follower" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr childact, act, dispatch ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;
            auto ls = phaserrobot->getLightSensor() ;

            act = std::make_shared<TankDrivePowerAction>(*db, 0.2, 0.2, false) ;
            mode->pushSubActionPair(db, act) ;

            act = std::make_shared<LineDetectAction>(*ls, *db) ;
            mode->pushSubActionPair(db, act) ;
                
            act = std::make_shared<LineFollowAction>(*ls, *db, "linefollow:power", "linefollow:distance", "linefollow:adjust") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;
        }

        ActionSequencePtr PhaserAutoModeController::createStraightCharAutoMode() {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveCharAction>(*db, 3.0, 0.5) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }        

        ActionSequencePtr PhaserAutoModeController::createScrubCharAutoMode() {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveScrubCharAction>(*db, 3.0, 0.3, 0.5) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }  

        ActionSequencePtr PhaserAutoModeController::createTestOne() {
            std::string name = "Test One" ;
            std::string desc = "Drive straight" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;
            std::shared_ptr<TerminateAction> termptr ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            return mode ;         
        }

        ActionSequencePtr PhaserAutoModeController::createTestTwo() {
            return nullptr ;
        }        

        ActionSequencePtr PhaserAutoModeController::testHatchHolder() {
            std::string name = "Test Hatch Holder" ;
            std::string desc = "Cycle through the combinations of the hatch holder" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto hatchholder = phaserrobot->getGameManipulator()->getHatchHolder() ;

            act = std::make_shared<HatchHolderAction>(*hatchholder, HatchHolderAction::Operation::EXTEND_ARM) ;
            mode->pushSubActionPair(hatchholder, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<HatchHolderAction>(*hatchholder, HatchHolderAction::Operation::EXTEND_FINGER) ;
            mode->pushSubActionPair(hatchholder, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            mode->pushAction(act) ;           

            act = std::make_shared<HatchHolderAction>(*hatchholder, HatchHolderAction::Operation::RETRACT_ARM) ;
            mode->pushSubActionPair(hatchholder, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<HatchHolderAction>(*hatchholder, HatchHolderAction::Operation::RETRACT_FINGER) ;
            mode->pushSubActionPair(hatchholder, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            mode->pushAction(act) ;               

            return mode ;             
        }

        ActionPtr PhaserAutoModeController::lifterGoToHeight(double height) {
            ActionSequencePtr seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "lifter_go_to_height", "") ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            act = std::make_shared<LifterGoToHeightAction>(*lifter, height) ;
            seq->pushSubActionPair(lifter, act) ;

            act = std::make_shared<LifterHoldHeightAction>(*lifter, height) ;
            seq->pushSubActionPair(lifter, act, false) ;     

            return seq ;     
        }

        ActionPtr PhaserAutoModeController::turntableGoToAngle(double angle) {
            ActionSequencePtr seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "turntable_go_to_angle", "") ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, angle) ;
            seq->pushSubActionPair(turntable, act) ;

            act = std::make_shared<TurntableHoldAngleAction>(*turntable, angle) ;
            seq->pushSubActionPair(turntable, act, false) ;     

            return seq ;                 
        }        

        xero::base::ActionPtr PhaserAutoModeController::driveAndPosition(const std::string &path, double height, double angle) {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;               
            auto par = std::make_shared<ParallelAction>() ;
            std::shared_ptr<DispatchAction> dact ;
            ActionPtr act ;

            par->addAction(turntableGoToAngle(angle)) ;
            par->addAction(lifterGoToHeight(height)) ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "HabCenterCargoFrontLeft") ;
            dact = std::make_shared<DispatchAction>(db, act) ;
            par->addAction(dact) ;

            return par ;
        }
    }
}
