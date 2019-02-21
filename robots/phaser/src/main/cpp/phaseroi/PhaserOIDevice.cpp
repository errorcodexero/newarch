#include "PhaserOIDevice.h"
#include "PhaserOISubsystem.h"
#include "Phaser.h"
#include "phaserrobotsubsystem/PhaserRobotSubsystem.h"
#include <cameratracker/CameraChangeAction.h>
#include <ActionSequence.h>

#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/GamePieceAction.h"
#include "gamepiecemanipulator/FloorCollectCargoAction.h"
#include "gamepiecemanipulator/FloorCollectHatchAction.h"
#include "gamepiecemanipulator/CompleteLSHatchCollect.h"
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
            initialize() ;

            height_ = ActionHeight::LevelOne ;
            dir_ = Direction::North ;
            mode_ = OperationMode::Invalid ;     
        }

        PhaserOIDevice::~PhaserOIDevice() {
        }
        
        void PhaserOIDevice::initialize() {
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

            button = settings.getInteger("oi:score") ;
            score_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:climb") ;
            climb_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            button= settings.getInteger("oi:extra_button") ;
            extra_button_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
        }

        void PhaserOIDevice::createActions() {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;  
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;

            finish_collect_hatch_ = std::make_shared<CompleteLSHatchCollect>(*game) ;
            finish_place_hatch_  = std::make_shared<ScoreHatch>(*game) ;
            finish_place_cargo_  = std::make_shared<ScoreCargo>(*game) ;
            set_collect_hatch_floor_  = std::make_shared<FloorCollectHatchAction>(*game) ;
            set_collect_cargo_floor_  = std::make_shared<FloorCollectCargoAction>(*game) ;
            reset_intakes_ = std::make_shared<ResetIntakeAction>(*game) ;
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

        void PhaserOIDevice::updateMode(OperationMode mode) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;  
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;

            if ((mode_ == OperationMode::Manual || mode_ == OperationMode::SemiAuto) && mode == OperationMode::Auto)
            {
                // Switch camera to tracking mode
                auto act = std::make_shared<CameraChangeAction>(*camera, camera->getCameraIndex(), CameraTracker::CameraMode::TargetTracking) ;
                camera->setAction(act) ;         
            }
            else if (mode_ == OperationMode::Auto && (mode == OperationMode::Manual || mode == OperationMode::SemiAuto))
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

        bool PhaserOIDevice::getDirection() {
            bool ret = false ;

            if (getValue(compass_north_)) {
                dir_ = Direction::North ;
                ret = true ;
            }
            else if (getValue(compass_south_)) {
                dir_ = Direction::South ;
                ret = true ;                
            }
            else if (getValue(compass_east_)) {
                dir_ = Direction::East ;
                ret = true ;                
            }
            else if (getValue(compass_west_)) {
                dir_ = Direction::West ;
                ret = true ;                
            }

            return ret ;
        }

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

        void PhaserOIDevice::generateTargetHeightActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto lifter = game->getLifter() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;     
            ActionPtr act, finish ;
            std::string height ;        

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating height related actions" ;
            log.endMessage() ;                    

            if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                height = "lifter:height:hatch:place:" ;
                height += dirToString() + ":" ;
                height += heightToString() ;
            }
            else if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                height = "lifter:height:cargo:place:" ;
                height += dirToString() + ":" ;
                height += heightToString() ;
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

                    piece = GamePieceManipulator::GamePieceType::Cargo ;
                }
                else {
                    //
                    // Loading cargo from the loading station
                    //
                    height = "lifter:height:hatch:collect:" ;
                    height += dirToString()  ;
                }
            }  

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

        void PhaserOIDevice::generateDirectionActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;     
            ActionPtr act, finish ;
            std::string height, angle ;     

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating direction related actions" ;
            log.endMessage() ;                     

            if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                height = "lifter:height:hatch:place:" ;
                height += dirToString() + ":1" ;
                angle = "turntable:angle:hatch:place:" ;
                angle += dirToString() ;
            }
            else if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                height = "lifter:height:cargo:place:" ;
                height += dirToString() + ":1" ;
                angle = "turntable:angle:cargo:place:" ;
                angle += dirToString() ;
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

                    std::cout << "Cargo Hatch Switch is cargo" << std::endl ;
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

                    std::cout << "Cargo Hatch Switch is hatch" << std::endl ;                                                      
                }
            }  

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

        void PhaserOIDevice::generateHeightButtonActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;            
            bool targetheight = false ;
            bool vision = false ;
            bool linefollower = false ;

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: generating height button related actions" ;
            log.endMessage() ;                  

            if (mode_ == OperationMode::Auto) {
                vision = true ;
                linefollower = true ;
            }
            else if (mode_ == OperationMode::SemiAuto) {
                targetheight = true ;
                linefollower = true ;
            }
            else {
                targetheight = true ;
            }

            if (targetheight)
                generateTargetHeightActions(seq) ;

            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;
            teleop->clearDetectors() ;                

            if (vision)
                setupVisionDetectors() ;

            if (linefollower)
                setupLineFollowingDetectors() ;
        }

        void PhaserOIDevice::setupLineFollowingDetectors() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ; 
            auto db = ph.getPhaserRobotSubsystem()->getTankDrive() ;  
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            ActionPtr finish = getFinishAction() ;         
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ;        

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: enabling line follower detectors" ;
            log.endMessage() ;                 
            if (dir_ == Direction::North && teleop != nullptr) {
                auto line = ph.getPhaserRobotSubsystem()->getFrontLineSensor() ;
                ActionSequencePtr seq = std::make_shared<ActionSequence>(log) ;
                ActionPtr drive = std::make_shared<LineFollowAction>(*line, *db, "linefollower:front:power", "linefollower:front:distance", "linefollower:front:adjust") ;
                seq->pushSubActionPair(db, drive) ;
                seq->pushSubActionPair(game, finish) ;
                teleop->addDetector(std::make_shared<LineFollowerTakeover>(teleop, seq, *line)) ;
            }
            else if (dir_ == Direction::South && teleop != nullptr) {
                auto line = ph.getPhaserRobotSubsystem()->getBackLineSensor() ;                
                ActionSequencePtr seq = std::make_shared<ActionSequence>(log) ;
                ActionPtr drive = std::make_shared<LineFollowAction>(*line, *db, "linefollower:back:power", "linefollower:back:distance", "linefollower:back:adjust") ;
                seq->pushSubActionPair(db, drive) ;
                seq->pushSubActionPair(game, finish) ;                
                teleop->addDetector(std::make_shared<LineFollowerTakeover>(teleop, seq, *line)) ;
            }
        }

        void PhaserOIDevice::setupVisionDetectors() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;   
            auto ctrl = ph.getCurrentController() ;
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ; 

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: enabling vision detectors" ;
            log.endMessage() ;                 

            //
            // TODO: Assign a drive by vision action, as the finish action.  And remove the
            //       vision detector as it should no loger be processed.
            //

            auto db = ph.getPhaserRobotSubsystem()->getTankDrive() ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;

            //
            // TODO: This wont exactly work.  When the drive by vision action kicks in, it needs
            //       to remove the vision detector from the teleop controller
            //

            //
            ActionPtr act = std::make_shared<DriveByVisionAction>(*db, *camera) ;
            act = std::make_shared<DispatchAction>(db, act) ;

            teleop->addDetector(std::make_shared<VisionDetectTakeover>(teleop, act, *ph.getPhaserRobotSubsystem()->getCameraTracker())) ;
        }

        xero::base::ActionPtr PhaserOIDevice::getFinishAction() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;               
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;

            if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                // Place cargo
                return finish_place_cargo_ ;
            }
            else if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                // Place hatch
                return finish_place_hatch_ ;
            }    
            else if (getValue(hatch_cargo_switch_)) {
                // Collect cargo
                return finish_collect_cargo_ ;
            }                   
            else {
                // Collect hatch
                return finish_collect_hatch_ ;
            }     

            return nullptr ;
        }

        void PhaserOIDevice::manuallyFinish(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;               
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto ctrl = ph.getCurrentController() ;            
            std::shared_ptr<TeleopController> teleop = std::dynamic_pointer_cast<TeleopController>(ctrl) ; 

            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_PHASER_OI) ;
            log << "OI: manually finishing action" ;
            log.endMessage() ;              

            teleop->clearDetectors() ;
            seq.pushSubActionPair(game, getFinishAction()) ;
        }

        void PhaserOIDevice::getCargoHatchMode(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;               
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            size_t camerano = 0 ;

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

            if (set_collect_cargo_floor_ == nullptr)
                createActions() ;            

            getTrackingMode() ;
            getCargoHatchMode(seq) ;

            if (getValue(extra_button_)) {
                //
                // Pull any intakes that are out of the robot back into the
                // robot.
                //
                seq.pushSubActionPair(game, reset_intakes_) ;
            }
            else if (getDirection()) {
                //
                // The joystick has been pressed in a direction, move the
                // lift and turntable to match to desired target
                //
                generateDirectionActions(seq) ;
            }
            else if (getHeightButton()) {
                //
                // A height button has been pressed, ready the lift for the
                // operation
                //
                generateHeightButtonActions(seq) ;
            }
            else if (getValue(go_)) {
                //
                // Manually initiate the seleced operation
                //
                manuallyFinish(seq) ;
            }
            else if (getValue(collect_floor_)) {
                //
                // Collect game pieces from the floor
                //
                if (getValue(hatch_cargo_switch_))
                    game->setAction(set_collect_cargo_floor_) ;
                else
                    game->setAction(set_collect_hatch_floor_) ;                    
            }
        }
    }
}
