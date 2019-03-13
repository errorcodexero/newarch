#include "automodes/PhaserAutoModeController.h"
#include "automodes/StraightCharAutoMode.h"
#include "automodes/FollowPathAutomode.h"
#include "automodes/CenterHabTwoHatch.h"
#include "automodes/HabCargoMode.h"
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
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true) ;
                break ;

            case 1:
                break ;

            case 2:
                mode = std::make_shared<CenterHabTwoHatch>(getRobot(), true, false) ;                
                break ;

            case 3:
                break ;

            case 4:
                break ;

            case 5:
                break ;

            case 6:
                mode = std::make_shared<FollowPathAutomode>(getRobot(), paths, reverses) ;
                break ;

            case 7:
                mode = std::make_shared<LifterHeightMode>(getRobot()) ;
                break ;

            case 8:
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.5, 3.0) ;                 
                break ;

            case 9:
                mode = std::make_shared<TankDriveScrubMode>(getRobot(), 0.5, 6.0, true) ;          
                break ;
            }
            setAction(mode) ;
        }
    }
}
