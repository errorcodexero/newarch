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

using namespace xero::base ;

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

            // 
            // Actions
            // 
            collect_hatch_floor_ = mapButton(1, OIButton::ButtonType::LowtoHigh) ;
            collect_cargo_floor_ = mapButton(2, OIButton::ButtonType::LowtoHigh) ;   

            camera_switch_ = mapButton(15, OIButton::ButtonType::Level) ;   
            camera_mode_ = mapButton(14, OIButton::ButtonType::Level) ;   
        }

        void PhaserOIDevice::createActions() {
        }

        void PhaserOIDevice::generateActions(ActionSequence &seq) {
            Phaser &ph = dynamic_cast<Phaser &>(getSubsystem().getRobot()) ;
            auto camera = ph.getPhaserRobotSubsystem()->getCameraTracker() ;

            size_t which = 0 ;
            CameraTracker::CameraMode mode = CameraTracker::CameraMode::TargetTracking ;

            if (getValue(camera_switch_) == 1)
                which = 1 ;
            
            if (getValue(camera_mode_) == 1)
                mode = CameraTracker::CameraMode::DriverViewing ;

            ActionPtr action = std::make_shared<CameraChangeAction>(*camera, which, mode) ;
            seq.pushSubActionPair(camera, action) ;            
        }
    }
}
