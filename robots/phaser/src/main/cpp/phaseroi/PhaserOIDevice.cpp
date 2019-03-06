#include "PhaserOIDevice.h"
#include "PhaserOISubsystem.h"
#include "Phaser.h"
#include "phaserrobotsubsystem/PhaserRobotSubsystem.h"
#include <cameratracker/CameraChangeAction.h>
#include <ActionSequence.h>
#include <ParallelAction.h>
#include "gamepiecemanipulator/GamePieceAction.h"
#include "gamepiecemanipulator/FloorCollectCargoAction.h"
#include "gamepiecemanipulator/FloorCollectHatchAction.h"
#include "gamepiecemanipulator/CompleteLSHatchCollect.h"
#include "gamepiecemanipulator/CompleteLSCargoCollect.h"
#include "gamepiecemanipulator/ScoreCargo.h"
#include "gamepiecemanipulator/ScoreHatch.h"
#include "gamepiecemanipulator/ReadyAction.h"
#include "gamepiecemanipulator/ResetIntakesAction.h"
#include "cargointake/CargoIntakeAction.h"
#include "singlemotorsubsystem/SingleMotorPowerAction.h"
#include "phasercameratracker/DriveByVisionAction.h"
#include "lifter/LifterPowerAction.h"
#include "lifter/LifterGoToHeightAction.h"
#include "tankdrive/LineFollowAction.h"
#include "hatchholder/HatchHolderAction.h"
#include "phaserrobotsubsystem/PhaserRobotSubsystem.h"
#include "phaserrobotsubsystem/ClimbAction.h"
#include "climber/ClimberDeployAction.h"
#include <oi/DriverGamepadRumbleAction.h>

#include "LineFollowerTakeover.h"
#include "VisionDetectTakeover.h"
#include "phaserids.h"

#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {

        PhaserOIDevice::PhaserOIDevice(PhaserOISubsystem &sub, int index) : OIDevice(sub, index) {
            //
            // Bind keys joystick buttons and axis to meaningful OI items
            //
            bindOI() ;

            //
            // Initialize state maintained by the OI
            //
            height_ = ActionHeight::LevelOne ;
            dir_ = Direction::North ;
            mode_ = OperationMode::Invalid ;     

            has_hatch_state_ = false ;
        }

        PhaserOIDevice::~PhaserOIDevice() {
        }

        std::string PhaserOIDevice::toString(OperationMode mode) {
            std::string ret = "?????" ;

            switch(mode) {
                case OperationMode::Auto:
                    ret = "Auto" ;
                    break ;
                case OperationMode::Invalid:
                    ret = "Invalid" ;
                    break ;
                case OperationMode::Manual:
                    ret = "Manual" ;
                    break ;
                case OperationMode::SemiAuto:
                    ret = "SemiAuto" ;
                    break ;
            }

            return ret ;
        }        

        std::string PhaserOIDevice::dirToString() {
            std::string ret = "????" ;

            switch(dir_) {
            case Direction::North:
                ret = "north" ;
                break ;

            case Direction::South:
                ret = "south" ;
                break ;

            case Direction::East:
                ret = "east" ;
                break ;

            case Direction::West:
                ret = "west" ;
                break ;                                                
            }

            return ret ;
        }        

        std::string PhaserOIDevice::heightToString() {
            std::string ret = "????" ;

            switch(height_) {
            case ActionHeight::LevelOne:
                ret = "1" ;
                break ;

            case ActionHeight::LevelTwo:
                ret = "2" ;
                break ;

            case ActionHeight::LevelThree:
                ret = "3" ;
                break ;

            case ActionHeight::CargoBay:
                ret = "c" ;
                break ;                                                
            }

            return ret ;
        }          

        //
        // Update the mode of the robot.  This should changes the camera mode if necessary
        //
        void PhaserOIDevice::updateMode(OperationMode mode) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;  
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;

            if ((mode_ == OperationMode::Manual || mode_ == OperationMode::SemiAuto || mode_ == OperationMode::Invalid) && mode == OperationMode::Auto)
            {
                // Switch camera to tracking mode
                auto act = std::make_shared<CameraChangeAction>(*camera, camera->getCameraIndex(), CameraTracker::CameraMode::TargetTracking) ;
                camera->setAction(act) ;         
            }
            else if (mode_ == OperationMode::Auto && (mode == OperationMode::Manual || mode == OperationMode::SemiAuto || mode_ == OperationMode::Invalid))
            {
                // Switch camera to viewing mode
                auto act = std::make_shared<CameraChangeAction>(*camera, camera->getCameraIndex(), CameraTracker::CameraMode::DriverViewing) ;
                camera->setAction(act) ;
            }

            std::string newmode = toString(mode) ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: updating mode" ;
            log << ", old mode " << toString(mode_) ;
            log << ", new mode " << newmode ; ;
            log.endMessage() ;            

            mode_ = mode ;
            frc::SmartDashboard::PutString("RobotMode", newmode) ;
        }        
        
        //
        // Map buttons, switches, joysticks, axis, etc. to things meaningful to the OI
        //
        void PhaserOIDevice::bindOI() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;   
            int button ;
            SettingsParser &settings = getSubsystem().getRobot().getSettingsParser() ;
            
            button = settings.getInteger("oi:hatch_cargo_switch") ;
            hatch_cargo_switch_ = mapButton(button, OIButton::ButtonType::LevelInv) ;

            button = settings.getInteger("oi:tracking_viewing_nothing_switch") ;
            tracking_manual_switch_ = mapAxisSwitch(button, 3) ;
            
            button = settings.getInteger("oi:climb_lock_switch") ;
            climb_lock_switch_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            button = settings.getInteger("oi:compass_north") ;
            compass_north_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:compass_south") ;
            compass_south_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:compass_east") ;
            compass_east_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:compass_west") ;
            compass_west_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:height_level_one") ;
            height_level_one_ = mapButton(button, OIButton::ButtonType::LowToHigh) ; 

            button = settings.getInteger("oi:height_level_two") ;
            height_level_two_ = mapButton(button, OIButton::ButtonType::LowToHigh) ; 

            button = settings.getInteger("oi:height_level_three") ;
            height_level_three_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:height_cargo_bay") ;
            height_cargo_bay_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:collect_floor") ;
            collect_floor_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:go") ;
            go_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button = settings.getInteger("oi:turtle") ;
            turtle_mode_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:climb") ;
            climb_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:calibrate") ;
            calibrate_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
        }

        //
        // Create static actions we for the OI
        //
        void PhaserOIDevice::init() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;  
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto hatch_holder = game->getHatchHolder() ;
            auto climber = ph.getPhaserRobotSubsystem()->getClimber() ;

            finish_collect_hatch_ = std::make_shared<CompleteLSHatchCollect>(*game) ;
            finish_collect_cargo_ = std::make_shared<CompleteLSCargoCollect>(*game) ;
            finish_place_hatch_  = std::make_shared<ScoreHatch>(*game) ;
            finish_place_cargo_  = std::make_shared<ScoreCargo>(*game) ;
            // set_collect_hatch_floor_  = std::make_shared<FloorCollectHatchAction>(*game) ;
            set_collect_cargo_floor_  = std::make_shared<FloorCollectCargoAction>(*game) ;
            reset_intakes_ = std::make_shared<ResetIntakeAction>(*game) ;
            extend_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER, "hatchholder:default:delay") ;
            retract_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER, "hatchholder:default:delay") ;     
            climb_action_ = std::make_shared<ClimbAction>(*ph.getPhaserRobotSubsystem()) ;

            deploy_climber_ = std::make_shared<ClimberDeployAction>(*climber) ;
        }

        //
        // See if a direction is being pressed on the joystick and if so, store the
        // direction and return true.
        //
        bool PhaserOIDevice::getDirection() {
            bool ret = false ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            if (getValue(compass_north_)) {
                dir_ = Direction::North ;
                ret = true ;
                log << "OI: North" ;
            }
            else if (getValue(compass_south_)) {
                dir_ = Direction::South ;
                ret = true ;                
                log << "OI: South" ;
            }
            else if (getValue(compass_east_)) {
                dir_ = Direction::East ;
                ret = true ;             
                log << "OI: East" ;                   
            }
            else if (getValue(compass_west_)) {
                dir_ = Direction::West ;
                ret = true ;            
                log << "OI: West" ;                    
            }
            log.endMessage() ;

            return ret ;
        }

        //
        // See if a height button is being pressed on the OI.  If so, store the height
        // and return true.
        //
        bool PhaserOIDevice::getHeightButton() {
            bool ret = false ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
  
            if (getValue(height_level_one_)) {
                height_ = ActionHeight::LevelOne ;
                ret = true ;
                log << "OI: Level 1" ;
            }
            else if (getValue(height_level_two_)) {
                height_ = ActionHeight::LevelTwo ;
                ret = true ;
                log << "OI: Level 2" ;                
            }
            else if (getValue(height_level_three_)) {
                height_ = ActionHeight::LevelThree ;
                ret = true ;
                log << "OI: Level 3" ;                
            }
            else if (getValue(height_cargo_bay_)) {
                height_ = ActionHeight::CargoBay ;
                ret = true ;
                log << "OI: Cargo Height" ;                
            }
            log.endMessage() ; 
            return ret ;
        }

        //
        // Read the mode switch.  If it differs from the current mode, call UpdateMode()
        // to update the current mode.
        //
        void PhaserOIDevice::getTrackingMode() {
            OperationMode mode = OperationMode::Auto ;
            int value = getValue(tracking_manual_switch_) ;

            switch(value) {
            case 0:
                mode = OperationMode::Auto ;
                break ;
            case 1:
                mode = OperationMode::SemiAuto ;
                break ;            
            case 2:
                mode = OperationMode::Manual ;
                break ;            
            }        
            if (mode != mode_)
                updateMode(mode) ;                
        }


        //
        // Generate the actions associated with a direction button on the joystick.  The
        // generated action raises the lift to a safe height, rotates the turntable to the
        // desired angle based on the action to be performed, and then sets the lift at a
        // level one height to ready for the operation.
        //
        void PhaserOIDevice::generateDirectionActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;     
            ActionPtr act, finish ;
            std::string height, angle ;     

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating direction related actions: " ;

            if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                if (mode_ == OperationMode::Auto) {
                    //
                    // The robot is holding a hatch, the action will be to place a hatch
                    // at the requested height
                    //
                    height = "lifter:height:hatch:tracking_height:" ;
                    height += dirToString() ;
                    angle = "turntable:angle:hatch:place:" ;
                    angle += dirToString() ;                    
                }
                else {                
                    height = "lifter:height:hatch:place:" ;
                    height += dirToString() + ":1" ;
                    angle = "turntable:angle:hatch:place:" ;
                    angle += dirToString() ;
                }
            }
            else if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                if (mode_ == OperationMode::Auto) {
                    //
                    // The roboti sholding cargo and we are in auto mode, set the
                    // height so that the camera can see the vision targets.
                    //
                    height = "lifter:height:cargo:tracking_height:" ;
                    height += dirToString() + ":" ;
                    height += heightToString() ;
                    angle = "turntable:angle:cargo:place:" ;
                    angle += dirToString() ;                    
                }
                else {
                    //
                    // The robot is holding cargo, the action will be to place the cargo
                    // at the requested height
                    //
                    height = "lifter:height:cargo:place:" ;
                    height += dirToString() + ":1" ;
                    angle = "turntable:angle:cargo:place:" ;
                    angle += dirToString() ;                      
                }
            }
            else if (piece == GamePieceManipulator::GamePieceType::None) {
                //
                // We are going to collect, read the hatch/cargo switch
                //
                if (getValue(hatch_cargo_switch_)) {
                    //
                    // Loading cargo from the loading station
                    //
                    height = "lifter:height:cargo:collect:" ;
                    height += dirToString() ;

                    angle = "turntable:angle:cargo:collect:" ;
                    angle += dirToString() ;

                    piece = GamePieceManipulator::GamePieceType::Cargo ;
                }
                else {
                    //
                    // Loading hatch from the loading station
                    //
                    height = "lifter:height:hatch:collect:" ;
                    height += dirToString()  ;

                    angle = "turntable:angle:hatch:collect:" ;
                    angle += dirToString() ;  

                    piece = GamePieceManipulator::GamePieceType::Hatch ;    
                }
            }  
            log << "height " << height ;
            log << ", angle " << angle ;
            log.endMessage() ;

            SettingsParser &parser = getSubsystem().getRobot().getSettingsParser() ;
            if (parser.isDefined(height) && parser.isDefined(angle)) {
                act = std::make_shared<ReadyAction>(*game, height, angle) ;
                seq.pushSubActionPair(game, act, false) ;
            }
            else {
                MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
                if (!parser.isDefined(height)) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "Missing expected param file value '" ;
                    logger << height ;
                    logger << "'" ;
                    logger.endMessage() ;
                }

                if (!parser.isDefined(angle)) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "Missing expected param file value '" ;
                    logger << angle ;
                    logger << "'" ;
                    logger.endMessage() ;
                }
            }
        }

        //
        // Generate the name of the height parameter in the params file for the operation we are
        // trying to complete.  IT takes into account place/collect, mode (auto, semi, manual), 
        // height, and cargo/hatch.
        //
        std::string PhaserOIDevice::generateActionHeightName() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto lifter = game->getLifter() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;  

            std::string height ;

            if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                if (mode_ == OperationMode::Auto) {
                    //
                    // The robot is holding a hatch and we are in auto mode, set the
                    // height so that the camera can see the vision targets
                    //
                    height = "lifter:height:hatch:tracking_height:" ;
                    height += dirToString() ;
                }
                else {
                    //
                    // The robot is holding a hatch, the action will be to place a hatch
                    // at the requested height
                    //
                    height = "lifter:height:hatch:place:" ;
                    height += dirToString() + ":" ;
                    height += heightToString() ;
                }
            }
            else if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                if (mode_ == OperationMode::Auto) {
                    //
                    // The roboti sholding cargo and we are in auto mode, set the
                    // height so that the camera can see the vision targets.
                    //
                    height = "lifter:height:cargo:tracking_height:" ;
                    height += dirToString() + ":" ;
                    height += heightToString() ;
                }
                else {
                    //
                    // The robot is holding cargo, the action will be to place the cargo
                    // at the requested height
                    //
                    height = "lifter:height:cargo:place:" ;
                    height += dirToString() + ":" ;
                    height += heightToString() ;
                }
            }
            else if (piece == GamePieceManipulator::GamePieceType::None) {
                //
                // We are going to collect, read the hatch/cargo switch
                //
                if (getValue(hatch_cargo_switch_)) {
                    //
                    // Loading hatch from the loading station
                    //
                    height = "lifter:height:cargo:collect:" ;
                    height += dirToString() ;
                }
                else {
                    //
                    // Loading cargo from the loading station
                    //
                    height = "lifter:height:hatch:collect:" ;
                    height += dirToString()  ;
                }
            }

            return height ;
        }        

        //
        // Generate actions assocaited with the current height.  This assumes the 
        // turntable is already positioned.  This just rasies the lift to the
        // right height based on the action to perform.
        //
        void PhaserOIDevice::generateTargetHeightActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto lifter = game->getLifter() ;
            ActionPtr act, finish ;
            std::string height ;        

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating height related actions: " ;
            log << "mode " << toString(mode_) << " " ;
          
            height = generateActionHeightName() ;

            log << height ;
            log.endMessage() ;                      

            SettingsParser &parser = getSubsystem().getRobot().getSettingsParser() ;
            if (parser.isDefined(height)) {
                act = std::make_shared<LifterGoToHeightAction>(*lifter, height) ;
                seq.pushSubActionPair(lifter, act, false) ;
            }
            else {
                MessageLogger &logger = getSubsystem().getRobot().getMessageLogger() ;
                if (!parser.isDefined(height)) {
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "Missing expected param file value '" ;
                    logger << height ;
                    logger << "'" ;
                    logger.endMessage() ;
                }
            }
        }        

        //
        // Generates all of the actions associated with the height button
        //
        void PhaserOIDevice::generateHeightButtonActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;            
            bool targetheight = false ;
            bool vision = false ;
            bool linefollower = false ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating height button actions:" ;
               
            if (mode_ == OperationMode::Auto) {
                vision = true ;
                linefollower = true ;
                log << " vision_detector" ;
                log << " linefollower_detector" ;
                log << " lifter_height" ;
            }
            else if (mode_ == OperationMode::SemiAuto) {
                targetheight = true ;
                linefollower = true ;
                log << " linefollower_detector" ;
                log << " lifter_height" ;                
            }
            else {
                targetheight = true ;
                log << " lifter_height" ;                  
            }
            log.endMessage() ;               

            if (targetheight)
                generateTargetHeightActions(seq) ;

            //
            // Always clear out any existing detectors in the teleop controller
            //
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
            teleop->clearDetectors() ;                

            if (vision)
                setupVisionDetectors() ;

            if (linefollower)
                setupLineFollowingDetectors() ;
        }

        //
        // Setup detector that will take over if the line follower detects a line.
        //
        void PhaserOIDevice::setupLineFollowingDetectors() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ; 
            auto db = ph.getPhaserRobotSubsystem()->getTankDrive() ;  
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto lifter = game->getLifter() ;
            auto oi = ph.getOI() ;
            ActionPtr finish = getFinishAction() ;         
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;    
            MessageLogger &log = ph.getMessageLogger() ;    
            ActionSequencePtr seq = std::make_shared<ActionSequence>(log) ;
            std::shared_ptr<LightSensorSubsystem> line ;
            std::shared_ptr<LineFollowAction> drive ;
            std::shared_ptr<ParallelAction> parallel = std::make_shared<ParallelAction>() ;

            if (dir_ == Direction::North && teleop != nullptr) {
                line = ph.getPhaserRobotSubsystem()->getFrontLineSensor() ;
                drive = std::make_shared<LineFollowAction>(*line, *db, "linefollower:front:power", "linefollower:front:distance", "linefollower:front:adjust") ;

            }
            else if (dir_ == Direction::South && teleop != nullptr) {
                line = ph.getPhaserRobotSubsystem()->getBackLineSensor() ;                
                drive = std::make_shared<LineFollowAction>(*line, *db, "linefollower:back:power", "linefollower:back:distance", "linefollower:back:adjust") ;
            }

            std::string height = generateActionHeightName() ;
            ActionPtr lift = std::make_shared<LifterGoToHeightAction>(*lifter, height) ;

            ActionPtr rumble = std::make_shared<DriverGamepadRumbleAction>(*oi, false, 2, 1.0, 0.5) ;

            seq->pushSubActionPair(db, drive) ;
            seq->pushSubActionPair(game, finish) ;

            parallel->addAction(seq) ;
            parallel->addSubActionPair(lifter, lift) ;
            parallel->addSubActionPair(oi, rumble) ;

            teleop->addDetector(std::make_shared<LineFollowerTakeover>(teleop, parallel, *line)) ;            
        }

        //
        // Setup detector that will take over if the vision detects a target that meets the
        // takeover critera.  For now, this assumes that the vision target will cause the
        // line followers to take over.  We may want to enhance this in the future so that if the
        // DriveByVision finished, we execute the finish action.
        //
        void PhaserOIDevice::setupVisionDetectors() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ; 
            auto oi = ph.getOI() ;
            auto db = ph.getPhaserRobotSubsystem()->getTankDrive() ;  
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            std::shared_ptr<ParallelAction> parallel = std::make_shared<ParallelAction>() ;
            
            ActionPtr finish = getFinishAction() ;
            ActionPtr drive ;
            auto ctrl = ph.getCurrentController() ;

            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;    
            MessageLogger &log = ph.getMessageLogger() ;    

            ActionSequencePtr seq = std::make_shared<ActionSequence>(log) ;
            drive = std::make_shared<DriveByVisionAction>(*db, *camera) ;

            ActionPtr rumble = std::make_shared<DriverGamepadRumbleAction>(*oi, false, 3, 1.0, 0.3333333) ;            

            seq->pushSubActionPair(db, drive) ;
            seq->pushSubActionPair(game, finish) ;

            parallel->addAction(seq) ;
            parallel->addSubActionPair(oi, rumble) ;

            teleop->addDetector(std::make_shared<VisionDetectTakeover>(teleop, parallel, *camera)) ;
        }

        //
        // Get the finish action for the current mode.  The finish action assumes the turntable
        // and the lifter are at the right place.
        //
        xero::base::ActionPtr PhaserOIDevice::getFinishAction() {
            ActionPtr ret = nullptr ;;
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;               
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;

            if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                // Place cargo
                ret = finish_place_cargo_ ;
            }
            else if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                // Place hatch
                ret = finish_place_hatch_ ;
            }    
            else if (getValue(hatch_cargo_switch_)) {
                // Collect cargo
                ret = finish_collect_cargo_ ;
            }                   
            else {
                // Collect hatch
                ret = finish_collect_hatch_ ;
            }     

            return ret ;
        }

        //
        // Manually finish an action if a detector has not taken over and
        // finished the action.
        //
        void PhaserOIDevice::manuallyFinish(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;               
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto ctrl = ph.getCurrentController() ;            
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ; 
            ActionPtr act = getFinishAction() ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: manually finishing action:" ;
            log << act->toString() ;
            log.endMessage() ;              

            teleop->clearDetectors() ;
            seq.pushSubActionPair(game, act) ;
        }

        //
        // Get the cargo/hatch mode based on any game piece that is in the robot as well
        // as the cargo/hatch switch
        //
        void PhaserOIDevice::getCargoHatchMode(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;               
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            size_t camerano = HatchCamera ;

            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;         

            if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                camerano = CargoCamera ;
            }   
            else if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                camerano = HatchCamera ;
            }   
            else {
                if (getValue(hatch_cargo_switch_)) {
                    camerano = CargoCamera ;
                }
                else {
                    camerano = HatchCamera ;
                }
            }

            if (camera->getCameraIndex() != camerano) {
                MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
                log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
                log << "OI: getCargoHatchMode - changing camera to " ;
                if (camerano == HatchCamera)
                    log << "hatch" ;
                else
                    log << "cargo" ;
                log.endMessage() ;     
                            
                ActionPtr ptr = std::make_shared<CameraChangeAction>(*camera, camerano, camera->getCameraMode()) ;
                seq.pushSubActionPair(camera, ptr) ;
            }
        }

        void PhaserOIDevice::generateActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto hatch_holder = game->getHatchHolder() ;     
    
            //
            // Get the tracking mode, either auto, semi-auto, or manual
            //
            getTrackingMode() ;

            //
            // Get the cargo/hatch mode
            //
            getCargoHatchMode(seq) ;

            //
            // Cannot execute turtle mode if we are already executing turtle mode, or if we have
            // triggered the climb sequence.
            //
            if (getValue(turtle_mode_) && reset_intakes_->isDone() && climb_action_->isDone()) {
                //
                // Directly assign the action and make it forcing to ensure this
                // action takes priority over everything else.
                //
                game->setAction(reset_intakes_, true) ;
                hatch_finger_start_ = getSubsystem().getRobot().getTime() ;
            }
            else if (game->isDone()) {
                if (getValue(climb_lock_switch_) && getValue(climb_)) {
                    //
                    // Climb lock switch is off and the climb button is pushed.  
                    //
                    // Time to fly .... like a grasshopper
                    //
                    auto robotsub = ph.getPhaserRobotSubsystem() ;
                    robotsub->setAction(climb_action_) ;
                }
                else if (getDirection()) {
                    //
                    // The joystick has been pressed in a direction, move the
                    // lift and turntable to match to desired target
                    //
                    generateDirectionActions(seq) ;
                    hatch_finger_start_ = getSubsystem().getRobot().getTime() ;                    
                }
                else if (getHeightButton()) {
                    //
                    // A height button has been pressed, ready the lift for the
                    // operation
                    //
                    generateHeightButtonActions(seq) ;
                    hatch_finger_start_ = getSubsystem().getRobot().getTime() ;                                    
                }
                else if (getValue(go_)) {
                    //
                    // Manually initiate the seleced operation
                    //
                    manuallyFinish(seq) ;
                    hatch_finger_start_ = getSubsystem().getRobot().getTime() ;                     
                }
                else if (getValue(collect_floor_)) {
                    if (game->getGamePieceType() == GamePieceManipulator::GamePieceType::None) {
                        //
                        // Collect game pieces from the floor
                        //
                        if (getValue(hatch_cargo_switch_))
                            game->setAction(set_collect_cargo_floor_) ;
#ifdef NOTYET
                        else
                            game->setAction(set_collect_hatch_floor_) ;
#endif

                        hatch_finger_start_ = getSubsystem().getRobot().getTime() ;                             
                    }
                    else {
                        MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
                        log.startMessage(MessageLogger::MessageType::error) ;
                        log << "Pressed FloorCollect button while holding a game piece" ;
                        log.endMessage() ;                          
                    }
                }
            }
        }
    }
}
