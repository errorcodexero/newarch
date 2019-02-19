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
#include "gamepiecemanipulator/ReadyAction.h"

#include <Robot.h>
#include <SettingsParser.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserOIDevice::PhaserOIDevice(PhaserOISubsystem &sub, int index) : OIDevice(sub, index) {
            initialize() ;

            height_ = ActionHeight::LevelOne ;
            dir_ = Direction::North ;
        }

        PhaserOIDevice::~PhaserOIDevice() {
        }
        
        void PhaserOIDevice::initialize() {
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;   
            int button ;
            SettingsParser &settings = getSubsystem().getRobot().getSettingsParser() ;
            
            // 
            // Actions
            // 

            // switch
            button = settings.getInteger("oi:hatch_cargo_switch") ;
            hatch_cargo_switch_ = mapButton(button, OIButton::ButtonType::Level) ;

            button = settings.getInteger("oi:tracking_manual_switch") ;
            tracking_manual_switch_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            button = settings.getInteger("oi:climb_lock_switch") ;
            climb_lock_switch_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            button = settings.getInteger("oi:camera_switch") ;
            camera_switch_ = mapButton(button, OIButton::ButtonType::Level) ;   

            button = settings.getInteger("oi:camera_mode") ;
            camera_mode_ = mapButton(button, OIButton::ButtonType::Level) ;   

            // compass
            button = settings.getInteger("oi:compass_north") ;
            compass_north_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:compass_south") ;
            compass_south_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:compass_east") ;
            compass_east_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:compass_west") ;
            compass_west_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            // height
            button = settings.getInteger("oi:height_level_one") ;
            height_level_one_ = mapButton(button, OIButton::ButtonType::LowToHigh) ; 

            button = settings.getInteger("oi:height_level_two") ;
            height_level_two_ = mapButton(button, OIButton::ButtonType::LowToHigh) ; 

            button = settings.getInteger("oi:height_level_three") ;
            height_level_three_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:height_cargo_bay") ;
            height_cargo_bay_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            // collect
            button = settings.getInteger("oi:collect_floor") ;
            collect_floor_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:collect_loading_station") ;
            collect_loading_station_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            // score
            button = settings.getInteger("oi:score") ;
            score_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            // climb
            button= settings.getInteger("oi:climb") ;
            climb_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;

            // extra
            button= settings.getInteger("oi:extra_button") ;
            extra_button_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
        }

        void PhaserOIDevice::createActions() {
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

        bool PhaserOIDevice::getHeight() {
            bool ret = false ;

            if (getValue(height_level_one_)) {
                height_ = ActionHeight::LevelOne ;
                ret = true ;
            }
            else if (getValue(height_level_two_)) {
                height_ = ActionHeight::LevelTwo ;
                ret = true ;
            }
            else if (getValue(height_level_three_)) {
                height_ = ActionHeight::LevelThree ;
                ret = true ;
            }
            else if (getValue(height_cargo_bay_)) {
                height_ = ActionHeight::CargoBay ;
                ret = true ;
            }

            return ret ;
        }

        void PhaserOIDevice::generateReadyAction() {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            GamePieceManipulator::GamePieceType piece = game->getGamePieceType() ;     
            ActionPtr act ;      
            std::string height, angle ;             

            if (piece == GamePieceManipulator::GamePieceType::Hatch) {
                height = "lifter:height:hatch:place:" ;
                height += dirToString() + ":" ;
                height += heightToString() ;

                angle = "turntable:angle:hatch:place:" ;
                angle += dirToString() ;
            }
            else if (piece == GamePieceManipulator::GamePieceType::Cargo) {
                std::string height = "lifter:height:cargo:place:" ;
                height += dirToString() + ":" ;
                height += heightToString() ;

                std::string angle = "turntable:angle:cargo:place:" ;
                angle += dirToString() ;
            }
            else if (piece == GamePieceManipulator::GamePieceType::None) {
                //
                // We are going to collect, read the hatch/cargo switch
                //
                if (getValue(hatch_cargo_switch_)) {
                    //
                    // Loading hatch from the loading station
                    //
                    std::string height = "lifter:height:cargo:collect:" ;
                    height += dirToString() + ":" ;

                    std::string angle = "turntable:angle:cargo:collect:" ;
                    angle += dirToString() ;
                }
                else {
                    //
                    // Loading cargo from the loading station
                    //
                    std::string height = "lifter:height:hatch:collect:" ;
                    height += dirToString() + ":" ;

                    std::string angle = "turntable:angle:hatch:collect:" ;
                    angle += dirToString() ;                    
                }
            }  

            SettingsParser &parser = getSubsystem().getRobot().getSettingsParser() ;
            if (parser.isDefined(height) && parser.isDefined(angle)) {
                act = std::make_shared<ReadyAction>(*game, height, angle) ;
                game->setAction(act) ;
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

        void PhaserOIDevice::generateActions(ActionSequence &seq) {
            bool genact = false ;

            if (getDirection()) {
                genact = true ;
                height_ = ActionHeight::LevelOne ;
            }

            if (getHeight())
                genact = true ;

            if (genact)
                generateReadyAction() ;
        }
    }
}
