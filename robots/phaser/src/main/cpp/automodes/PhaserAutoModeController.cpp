#include "automodes/PhaserAutoModeController.h"
#include "automodes/StraightCharAutoMode.h"
#include "Phaser.h"

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
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.2, 10.0, true) ;
                break ;
            }
            setAction(mode) ;
        }
    }
}
