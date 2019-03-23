#include "CargoTrackerAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CargoTrackerAction::CargoTrackerAction(Turntable &subsystem, PhaserCameraTracker &vision):TurntableAction(subsystem), vision_(vision) 
        {
            kp_ = subsystem.getRobot().getSettingsParser().getDouble("cargotracking:kp") ;
        }

        CargoTrackerAction::~CargoTrackerAction(){
        }

        void CargoTrackerAction::start() {
        }

        void CargoTrackerAction::run() {
            if (vision_.isValid()) {
                double p = vision_.getYaw() * kp_ ;
                getTurntable().setMotorPower(p) ;
            }            
        }

        bool CargoTrackerAction::isDone() {
            return false ;
        }

        void CargoTrackerAction::cancel() {
        }

        std::string CargoTrackerAction::toString() {
            std::string ret("CargoTrackerAction") ;
            return ret ;
        }
    }
}