#include "automodes/PhaserAutoModeController.h"
#include "automodes/CenterHabTwoHatch.h"
#include "automodes/LeftRocketTwoHatch.h"
#include "automodes/RightRocketTwoHatch.h"

#include "automodes/LifterHeightMode.h"
#include "Phaser.h"

#include <tankdrive/modes/FollowPathAutomode.h>
#include <tankdrive/modes/TankDriveScrubMode.h>
#include <tankdrive/modes/StraightCharAutoMode.h>
#include <MessageLogger.h>

#include <frc/DriverStation.h>

using namespace xero::base ;
using namespace xero::misc ;

std::vector<std::string> paths = { "CenterHab2CargoFrontLeft", "CargoFrontLeftLSLeft"} ;
std::vector<bool> reverses = { false, true } ;

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
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true, false, "CenterHabSingleCargoLeft", "Start in center hab, score one on right front of cargo ship") ;
                break ;

            case 1:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true, true, "CenterHabTwoCargoLeftLS", "Start in center hab, score two on front of cargo ship, collect second from left side") ;
                break ;

            case 2:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false, false, "CenterHabSingleCargoRight", "Start in center hab, score one on left front of cargo ship") ;
                break ;

            case 3:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false, true, "CenterHabTwoCargoRightLS", "Start in center hab, score two on front of cargo ship, collect second from right side") ;
                break ;

            case 4:
                mode = std::make_shared<LeftRocketTwoHatch>(getRobot(), true) ;
                break ;

            case 5:
                mode = std::make_shared<LeftRocketTwoHatch>(getRobot(), false) ;            
                break ;

            case 6:
                mode = std::make_shared<RightRocketTwoHatch>(getRobot(), true) ;            
                break ;

            case 7:
                mode = std::make_shared<RightRocketTwoHatch>(getRobot(), false) ;             
                break ;

            case 8:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "Tune_Tune") ;
                break ;

            case 9:
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.4, 10.0) ;
                break ;
            }
            setAction(mode) ;
        }
    }
}
