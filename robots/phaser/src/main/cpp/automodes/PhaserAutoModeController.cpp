#include "PhaserAutoModeController.h"
#include "Phaser.h"
#include <tankdrive/TankDriveFollowPathAction.h>
#include <frc/DriverStation.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        PhaserAutoModeController::PhaserAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        PhaserAutoModeController::~PhaserAutoModeController() {
        }

        void PhaserAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            ActionSequencePtr mode = nullptr ;
            mode = createHabTwoHatchLeftLS() ;            
            setAction(mode) ;
        }

        ActionSequencePtr PhaserAutoModeController::createHabTwoHatchLeftLS() {
            std::string name = "CenterHabTwoHatch" ;
            std::string desc = "Start in the center of the HAB, score both center hatches getting second hatch from left automode" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "HabCenterCargoCenterLeft") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }
    }
}
