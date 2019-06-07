#include "PhoenixAutoController.h"
#include <frc/DriverStation.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixAutoController::PhoenixAutoController(Robot &robot) : AutoController(robot) {
        }
        
        PhoenixAutoController::~PhoenixAutoController() {
        }

        void PhoenixAutoController::updateAutoMode(int sel, const std::string &gamedata) {
            setAction(nullptr) ;
        }
    }
}
