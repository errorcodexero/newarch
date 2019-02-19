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

#include "lifter/LifterPowerAction.h"

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

            button = settings.getInteger("oi:tracking_viewing_nothing_switch") ;
            tracking_manual_switch_ = mapAxisSwitch(button, 3) ;
            
            button = settings.getInteger("oi:climb_lock_switch") ;
            climb_lock_switch_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            // compass
            button = settings.getInteger("oi:compass_north") ;
            compass_north_ = mapButton(button, OIButton::ButtonType::Level) ;

            button = settings.getInteger("oi:compass_south") ;
            compass_south_ = mapButton(button, OIButton::ButtonType::Level) ;

            button = settings.getInteger("oi:compass_east") ;
            compass_east_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            button = settings.getInteger("oi:compass_west") ;
            compass_west_ = mapButton(button, OIButton::ButtonType::Level) ;

            // height
            button = settings.getInteger("oi:height_level_one") ;
            height_level_one_ = mapButton(button, OIButton::ButtonType::Level) ; 

            button = settings.getInteger("oi:height_level_two") ;
            height_level_two_ = mapButton(button, OIButton::ButtonType::Level) ; 

            button = settings.getInteger("oi:height_level_three") ;
            height_level_three_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            button = settings.getInteger("oi:height_cargo_bay") ;
            height_cargo_bay_ = mapButton(button, OIButton::ButtonType::Level) ;
            
            // collect
            button = settings.getInteger("oi:collect_floor") ;
            std::cout << "Collect floor " << button << std::endl ;
            collect_floor_ = mapButton(button, OIButton::ButtonType::LowToHigh) ;
            
            button = settings.getInteger("oi:collect_loading_station") ;
            collect_loading_station_ = mapButton(button, OIButton::ButtonType::Level) ;

            // score
            button = settings.getInteger("oi:score") ;
            score_ = mapButton(button, OIButton::ButtonType::Level) ;

            // climb
            button= settings.getInteger("oi:climb") ;
            climb_ = mapButton(button, OIButton::ButtonType::Level) ;

            // extra
            button= settings.getInteger("oi:extra_button") ;
            extra_button_ = mapButton(button, OIButton::ButtonType::Level) ;
        }

        void PhaserOIDevice::createActions() {
        }

        void PhaserOIDevice::generateActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;
            auto game = ph.getPhaserRobotSubsystem()->getGameManipulator() ;
            ActionPtr action ; 

            size_t which = 0 ;
            CameraTracker::CameraMode mode = CameraTracker::CameraMode::TargetTracking ;

            if (getValue(camera_switch_) == 1)
                which = 1 ;
            
            if (getValue(camera_mode_) == 1)
                mode = CameraTracker::CameraMode::DriverViewing ;

            int floor = getValue(collect_floor_) ;
            if (floor) {
                std::cout << "Collect Floor" << std::endl ;
                action = std::make_shared<FloorCollectCargoAction>(*game) ;
                // seq.pushSubActionPair(game, action) ;
            }

            int north = getValue(compass_north_) ;
            int south = getValue(compass_south_) ;

            if (north) {
                action = std::make_shared<LifterPowerAction>(*game->getLifter(), 1.0) ;
                seq.pushSubActionPair(game->getLifter(), action) ;   
            }
            else if (south) {
                action = std::make_shared<LifterPowerAction>(*game->getLifter(), -0.2) ;
                seq.pushSubActionPair(game->getLifter(), action) ; 
            }
            else {
                action = std::make_shared<LifterPowerAction>(*game->getLifter(), 0.0) ;
                seq.pushSubActionPair(game->getLifter(), action) ;
            }

            action = std::make_shared<CameraChangeAction>(*camera, which, mode) ;
            seq.pushSubActionPair(camera, action) ;            
        }
    }
}
