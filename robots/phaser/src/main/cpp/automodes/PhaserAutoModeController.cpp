#include "automodes/PhaserAutoModeController.h"
#include "automodes/StraightCharAutoMode.h"
#include "automodes/FollowPathAutomode.h"
#include "automodes/CenterHabTwoHatch.h"
#include "automodes/HabCargoMode.h"
#include "automodes/TankDriveScrubMode.h"
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
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true) ;
                break ;

            case 1:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false) ;            
                break ;

            case 2:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false, false) ;                
                break ;

            case 3:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true, false) ;                 
                break ;

            case 4:
                mode = std::make_shared<HabCargoMode>(getRobot(), true) ;            
                break ;

            case 5:
                mode = std::make_shared<HabCargoMode>(getRobot(), false) ;               
                break ;

            case 6:
                break ;

            case 7:
                break ;

            case 8:
                break ;

            case 9:
                break ;
            }
            setAction(mode) ;
        }
    }
}
