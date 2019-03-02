#include "automodes/PhaserAutoModeController.h"
#include "automodes/StraightCharAutoMode.h"
#include "automodes/FollowPathAutomode.h"
#include "automodes/CenterHabTwoHatch.h"
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
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.5, 3.0) ;
                break ;

            case 1:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "Straight") ;
                break ;

            case 2:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "CurveLeft") ;            
                break ;

            case 3:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "CurveRight") ;
                break ;

            case 4:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true) ;
                break ;

            case 5:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false) ;            
                break ;

            case 6:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "CargoFrontLeftLSLeft", true) ;            
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
