#include "automodes/StraightCharAutoMode.h"
#include "Phaser.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveTimedPowerAction.h>
#include <lifter/LifterPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <DelayAction.h>
using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        StraightCharAutoMode::StraightCharAutoMode(Robot &robot) : PhaserAutoModeBase(robot, "StraightChar", "Characterize the drive base")
        {            
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto lifter = phaserrobot->getGameManipulator()->getLifter() ;

            pushSubActionPair(lifter, std::make_shared<LifterPowerAction>(*lifter, 0.5)) ;
            pushAction(std::make_shared<DelayAction>(2.5)) ;
            pushSubActionPair(lifter, std::make_shared<LifterPowerAction>(*lifter, 0.0)) ;            
        }

        StraightCharAutoMode::~StraightCharAutoMode()
        {            
        }
    }
}