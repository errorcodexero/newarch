#include "automodes/LifterHeightMode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveScrubCharAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <DelayAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        LifterHeightMode::LifterHeightMode(Robot &robot) : PhaserAutoModeBase(robot, "GoToHeight", "GoToHeight")
        {
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            pushSubActionPair(lifter, std::make_shared<LifterGoToHeightAction>(*lifter, 15)) ;
            pushAction(std::make_shared<DelayAction>(2.0)) ;
            pushSubActionPair(lifter, std::make_shared<LifterGoToHeightAction>(*lifter, -6, true)) ;
            pushAction(std::make_shared<DelayAction>(2.0)) ;            
            pushSubActionPair(lifter, std::make_shared<LifterGoToHeightAction>(*lifter, 44)) ;
            pushAction(std::make_shared<DelayAction>(2.0)) ;            
            pushSubActionPair(lifter, std::make_shared<LifterGoToHeightAction>(*lifter, -6, true)) ;
        }

        LifterHeightMode::~LifterHeightMode()
        {            
        }
    }
}