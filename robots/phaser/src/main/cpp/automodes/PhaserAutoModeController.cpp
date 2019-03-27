#include "automodes/PhaserAutoModeController.h"
#include "automodes/StraightCharAutoMode.h"
#include "automodes/FollowPathAutomode.h"
#include "automodes/CenterHabTwoHatch.h"
#include "automodes/LeftRocketTwoHatch.h"
#include "automodes/RightRocketTwoHatch.h"
#include "automodes/TankDriveScrubMode.h"
#include "automodes/LifterHeightMode.h"
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveScrubCharAction.h>
#include "Phaser.h"

#include <frc/DriverStation.h>
#include <MessageLogger.h>

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
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true, false, "CenterHabSingleCargo", "Start in center hab, score one on left front of cargo ship") ;
                break ;

            case 1:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true, true, "CenterHabTwoCargo", "Start in center hab, score two on front of cargo ship, collect second from left side") ;
                break ;

            case 2:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false, false, "CenterHabSingleCargo", "Start in center hab, score one on right front of cargo ship") ;
                break ;

            case 3:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), false, true, "CenterHabTwoCargo", "Start in center hab, score two on front of cargo ship, collect second from right side") ;
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
                break ;

            case 9:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), "LeftHABLeftRocket") ;
                break ;
            }
            setAction(mode) ;
        }
    }
}
