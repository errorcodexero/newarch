#include "PhaserAutoModeController.h"
#include "Phaser.h"
#include <frc/DriverStation.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        PhaserAutoModeController::PhaserAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        PhaserAutoModeController::~PhaserAutoModeController() {
        }

        void PhaserAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            Robot &robot = getRobot() ;
            Phaser &Phaser = dynamic_cast<xero::phaser::Phaser &>(robot) ;
            ActionSequencePtr mode = nullptr ;

            auto &ds = frc::DriverStation::GetInstance() ;
            std::string game = ds.GetGameSpecificMessage() ;
            char myswitch = game[0] ;

            switch(sel) {
            case 0:
                mode = nullptr ;
            }

            setAction(mode) ;
        }
    }
}
