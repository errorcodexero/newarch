#include "PhaserAutoModeController.h"
#include "Phaser.h"
#include "hatchholder/HatchHolderAction.h"
#include "phasercameratracker/DriveByVisionAction.h"
#include "cargoholder/CargoHolder.h"
#include "cargointake/CargoIntake.h"
#include "cargointake/CargoIntakeAction.h"
#include "hatchintake/HatchIntake.h"
#include "hatchintake/HatchIntakeAction.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/FloorCollectCargoAction.h"
#include "gamepiecemanipulator/FloorCollectHatchAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveScrubCharAction.h>
#include <tankdrive/LineDetectAction.h>
#include <tankdrive/LineFollowAction.h>
#include <tankdrive/TankDrivePowerAction.h>
#include <lifter/LifterCalibrateAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterPowerAction.h>
#include <turntable/TurntableCalibrateAction.h>
#include <turntable/TurntableGoToAngleAction.h>
#include <turntable/TurntablePowerAction.h>
#include <climber/Climber.h>
#include <climber/ClimberDeployAction.h>
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
            AutoModePtr mode = nullptr ;

            switch(sel) {
            case 0:
                break ;

            case 1:
                break ;

            case 2:
                break ;

            case 3:
                break ;

            case 4:
                break ;

            case 5:
                break ;

            case 6:
                break ;

            case 7:
                break ;

            case 8:
                break ;

            case 9:
                break ;
            }
            setAction(mode) ;
        }

        AutoModePtr PhaserAutoModeController::createStraightCharAutoMode(bool gear) {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveCharAction>(*db, 10.0, 0.2, gear) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }             

        AutoModePtr PhaserAutoModeController::testFloorCollectHatch() {
            std::string name = "Test Floor Collect Hatsh" ;
            std::string desc = "Test the floor collect hatch action" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto gamepiece = phaserrobot->getGameManipulator() ;

            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<LifterCalibrateAction>(*lifter) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<TurntableCalibrateAction>(*turntable) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<FloorCollectHatchAction>(*gamepiece) ;
            mode->pushSubActionPair(gamepiece, act) ;

            return mode ;            
        }

        AutoModePtr PhaserAutoModeController::testFloorCollectCargo() {
            std::string name = "Test Floor Collect Cargo" ;
            std::string desc = "Test the floor collect cargo action" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto gamepiece = phaserrobot->getGameManipulator() ;

            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<LifterCalibrateAction>(*lifter) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<TurntableCalibrateAction>(*turntable) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<FloorCollectCargoAction>(*gamepiece) ;
            mode->pushSubActionPair(gamepiece, act) ;

            return mode ;
        }

        AutoModePtr PhaserAutoModeController::testHatchHolder() {
            std::string name = "Test Hatch Holder" ;
            std::string desc = "Cycle through the combinations of the hatch holder" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
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

        AutoModePtr PhaserAutoModeController::testClimber() {
            std::string name = "Test Climber" ;
            std::string desc = "Test the climber" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto climber = phaserrobot->getClimber() ; 

            act = std::make_shared<DelayAction>(10.0) ;
            mode->pushAction(act) ;            

            act = std::make_shared<ClimberDeployAction>(*climber) ;
            mode->pushSubActionPair(climber, act) ;    

            return mode ;                                                    
        }        

        AutoModePtr PhaserAutoModeController::testCargoHolder() {
            std::string name = "Test Cargo Holder" ;
            std::string desc = "Test the cargo holder" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto cargoholder = phaserrobot->getGameManipulator()->getCargoHolder() ;    

            act = std::make_shared<SingleMotorPowerAction>(*cargoholder, 0.6) ;
            mode->pushSubActionPair(cargoholder, act) ;

            act = std::make_shared<DelayAction>(60.0) ;
            mode->pushAction(act) ;
            
            act = std::make_shared<SingleMotorPowerAction>(*cargoholder, 0.0) ;
            mode->pushSubActionPair(cargoholder, act) ;    

            return mode ;                                                    
        }

        AutoModePtr PhaserAutoModeController::testHatchIntake() {    
            std::string name = "Test Hatch Intake" ;
            std::string desc = "Test the hatch intake" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto hatchintake = phaserrobot->getGameManipulator()->getHatchIntake() ;   

            act = std::make_shared<HatchIntakeAction>(*hatchintake, true) ;
            mode->pushSubActionPair(hatchintake, act) ;  

            act = std::make_shared<DelayAction>(3.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<SingleMotorPowerAction>(*hatchintake, 0.2) ;
            mode->pushSubActionPair(hatchintake, act) ;

            act = std::make_shared<DelayAction>(3.0) ;
            mode->pushAction(act) ;      

            act = std::make_shared<SingleMotorPowerAction>(*hatchintake, 0.0) ;
            mode->pushSubActionPair(hatchintake, act) ;      

            act = std::make_shared<HatchIntakeAction>(*hatchintake, false) ;
            mode->pushSubActionPair(hatchintake, act) ;  

            return mode ;
        }      
        
        AutoModePtr PhaserAutoModeController::testCargoIntake() {    
            std::string name = "Test Cargo Intake" ;
            std::string desc = "Test the cargo intake" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto cargointake = phaserrobot->getGameManipulator()->getCargoIntake() ;    

            act = std::make_shared<CargoIntakeAction>(*cargointake, true) ;
            mode->pushSubActionPair(cargointake, act) ;

            act = std::make_shared<DelayAction>(3.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<SingleMotorPowerAction>(*cargointake, 0.2) ;
            mode->pushSubActionPair(cargointake, act) ;

            act = std::make_shared<DelayAction>(60.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<SingleMotorPowerAction>(*cargointake, 0.0) ;
            mode->pushSubActionPair(cargointake, act) ;

            act = std::make_shared<DelayAction>(60.0) ;
            mode->pushAction(act) ;       
            
            act = std::make_shared<CargoIntakeAction>(*cargointake, false) ;
            mode->pushSubActionPair(cargointake, act) ;       

            return mode ;                     
        }

        AutoModePtr PhaserAutoModeController::testLifterRaw() {
            std::string name = "Test Lifter Raw" ;
            std::string desc = "Test the lifter motor & sensor" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            act = std::make_shared<LifterCalibrateAction>(*lifter) ;
            mode->pushSubActionPair(lifter, act) ;            

            act = std::make_shared<LifterPowerAction>(*lifter, 0.40) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<DelayAction>(3.0) ;
            mode->pushAction(act) ;              

            act = std::make_shared<LifterPowerAction>(*lifter, 0.0) ;
            mode->pushSubActionPair(lifter, act) ;

            return mode ;                
        }


        AutoModePtr PhaserAutoModeController::testLifter() {
            std::string name = "Test Lifter" ;
            std::string desc = "Test the lifter go to height and hold" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            act = std::make_shared<LifterCalibrateAction>(*lifter) ;
            mode->pushSubActionPair(lifter, act) ;

            act = std::make_shared<LifterGoToHeightAction>(*lifter, 60.0) ;
            mode->pushSubActionPair(lifter, act) ;
            
            return mode ;                
        }

       AutoModePtr PhaserAutoModeController::testTurntableRaw() {
            std::string name = "Test Turntable Electrical" ;
            std::string desc = "Test the turntable electrical" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<TurntablePowerAction>(*turntable, 0.25) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<DelayAction>(2.0) ;
            mode->pushAction(act) ;
            
            act = std::make_shared<TurntablePowerAction>(*turntable, 0.0) ;
            mode->pushSubActionPair(turntable, act) ;                        

            return mode ;             
        }        

        AutoModePtr PhaserAutoModeController::testTurntable() {
            std::string name = "Test Turntable" ;
            std::string desc = "Test the turntable" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<TurntableCalibrateAction>(*turntable) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, 180) ;
            mode->pushSubActionPair(turntable, act) ;
            
#ifdef NOTYET
            act = std::make_shared<DelayAction>(5.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, -90) ;
            mode->pushSubActionPair(turntable, act) ;    

            act = std::make_shared<DelayAction>(5.0) ;
            mode->pushAction(act) ;
            
            act = std::make_shared<TurntableGoToAngleAction>(*turntable, 0) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<DelayAction>(5.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, -90) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<DelayAction>(5.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, 90) ;
            mode->pushSubActionPair(turntable, act) ;

            act = std::make_shared<DelayAction>(5.0) ;
            mode->pushAction(act) ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, 0) ;
            mode->pushSubActionPair(turntable, act) ; 
#endif           

            return mode ;             
        }


        AutoModePtr PhaserAutoModeController::testHatchDeposit() {
            return nullptr ;
        }        

        AutoModePtr PhaserAutoModeController::testVision() {
            std::string name = "Test Vision" ;
            std::string desc = "Test the vision system" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr childact, act, dispatch ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;
            auto cm = phaserrobot->getCameraTracker() ;

            act = std::make_shared<DriveByVisionAction>(*db, *cm) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;
        }         

        AutoModePtr PhaserAutoModeController::testLineFollower() {
            std::string name = "Test Line Follower" ;
            std::string desc = "Test the line follower" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr childact, act, dispatch ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;
            auto ls = phaserrobot->getFrontLineSensor() ;

            act = std::make_shared<TankDrivePowerAction>(*db, 0.2, 0.2, false) ;
            mode->pushSubActionPair(db, act) ;

            act = std::make_shared<LineDetectAction>(*ls, *db) ;
            mode->pushSubActionPair(db, act) ;
                
            act = std::make_shared<LineFollowAction>(*ls, *db, "linefollow:power", "linefollow:distance", "linefollow:adjust") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;
        }
   

        AutoModePtr PhaserAutoModeController::createScrubCharAutoMode() {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveScrubCharAction>(*db, 3.0, 0.3, 0.5) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }  

        AutoModePtr PhaserAutoModeController::createTestOne() {
            std::string name = "Test One" ;
            std::string desc = "Drive straight" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;
            std::shared_ptr<TerminateAction> termptr ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "Straight") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }

        AutoModePtr PhaserAutoModeController::createTestTwo() {
            std::string name = "Test One" ;
            std::string desc = "Drive straight" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;
            std::shared_ptr<TerminateAction> termptr ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto game = phaserrobot->getGameManipulator() ;

            act = std::make_shared<FloorCollectCargoAction>(*game) ;
            mode->pushSubActionPair(game, act) ;

            return mode ; 
        }        

        ActionPtr PhaserAutoModeController::lifterGoToHeight(double height) {
            AutoModePtr seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), "lifter_go_to_height", "") ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            act = std::make_shared<LifterGoToHeightAction>(*lifter, height) ;
            seq->pushSubActionPair(lifter, act) ;

            return seq ;     
        }

        ActionPtr PhaserAutoModeController::turntableGoToAngle(double angle) {
            AutoModePtr seq = std::make_shared<AutoMode>(getRobot().getMessageLogger(), "turntable_go_to_angle", "") ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto turntable = phaserrobot->getGameManipulator()->getTurntable() ;

            act = std::make_shared<TurntableGoToAngleAction>(*turntable, angle) ;
            seq->pushSubActionPair(turntable, act) ;

            return seq ;                 
        }        
    }
}
