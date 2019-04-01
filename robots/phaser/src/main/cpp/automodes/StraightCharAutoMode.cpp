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
        StraightCharAutoMode::StraightCharAutoMode(Robot &robot) : PhaserAutoModeBase(robot, "TestBackup", "Test the backup")
        {            
            auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
            auto phaserrobot = phaser.getPhaserRobotSubsystem() ;
            auto db = phaserrobot->getTankDrive() ;

            pushSubActionPair(db, std::make_shared<TankDriveTimedPowerAction>(*db, 0.2, 0.2, 0.30, true)) ;
        }

        StraightCharAutoMode::~StraightCharAutoMode()
        {            
        }
    }
}