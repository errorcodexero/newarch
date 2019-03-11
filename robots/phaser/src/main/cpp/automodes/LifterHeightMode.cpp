#include "automodes/LifterHeightMode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveScrubCharAction.h>
#include <lifter/LifterGoToHeightAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        LifterHeightMode::LifterHeightMode(Robot &robot, double height, bool rel) : PhaserAutoModeBase(robot, "GoToHeight", "GoToHeight")
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            pushSubActionPair(lifter, std::make_shared<LifterGoToHeightAction>(*lifter, height, rel)) ;
        }

        LifterHeightMode::~LifterHeightMode()
        {            
        }
    }
}