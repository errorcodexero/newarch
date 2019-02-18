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

#include <Robot.h>
#include <SettingsParser.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserOIDevice::PhaserOIDevice(PhaserOISubsystem &sub, int index) : OIDevice(sub, index) {
            initialize() ;
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

        void PhaserOIDevice::generateActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;

            size_t which = 0 ;
            CameraTracker::CameraMode mode = CameraTracker::CameraMode::TargetTracking ;

            if (getValue(camera_switch_) == 1)
                which = 1 ;
            
            if (getValue(camera_mode_) == 1)
                mode = CameraTracker::CameraMode::DriverViewing ;

            if (getValue(collect_floor_) == 1) {
                ActionPtr act = std::make_shared<FloorCollectCargoAction>(*game) ;
                seq.pushSubActionPair(game, act) ;
            }

            ActionPtr action = std::make_shared<CameraChangeAction>(*camera, which, mode) ;
            seq.pushSubActionPair(camera, action) ;            
        }
    }
}
