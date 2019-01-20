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
                mode = createTestOne() ;
                break ;

            case 2:
                mode = createTestTwo() ;
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

            act = std::make_shared<TankDriveCharAction>(*db, 2.5, 0.9) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }        

        ActionSequencePtr PhaserAutoModeController::createTestOne() {
            std::string name = "Test One" ;
            std::string desc = "Drive straight" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "HabLeftCargoLeftOne") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }

        ActionSequencePtr PhaserAutoModeController::createTestTwo() {
            return nullptr ;
        }        
    }
}
