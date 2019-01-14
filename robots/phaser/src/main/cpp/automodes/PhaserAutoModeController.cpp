#include "PhaserAutoModeController.h"
#include "Phaser.h"
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/TankDriveCharAction.h>
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

            switch(sel) {
            case 0:
                mode = createCharAutoMode() ;
                break ;

            case 1:
                mode = createHabTwoHatchLeftLS() ;
                break ;
            }
            setAction(mode) ;
        }

        ActionSequencePtr PhaserAutoModeController::createCharAutoMode() {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveCharAction>(*db, 4.0, 0.1) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
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
