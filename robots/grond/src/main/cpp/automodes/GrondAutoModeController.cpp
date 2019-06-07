#include "GrondAutoModeController.h"
#include "Grond.h"
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <frc/DriverStation.h>

using namespace xero::base ;

namespace xero {
    namespace grond {
        GrondAutoModeController::GrondAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        GrondAutoModeController::~GrondAutoModeController() {
        }

        void GrondAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            setAction(nullptr) ;
        }
    }
}
