#include "PhaserOISubsystem.h"
#include "PhaserOIDevice.h"
#include "Phaser.h"
#include "phaserrobotsubsystem/PhaserRobotSubsystem.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <DriverGamepad.h>
#include <ActionSequence.h>
#include <frc/DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserOISubsystem::PhaserOISubsystem(xero::base::Robot &robot) : OISubsystem(robot, "oi") {
            int oi = robot.getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  

            auto oidev = std::make_shared<PhaserOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        PhaserOISubsystem::~PhaserOISubsystem() {
        }

        bool PhaserOISubsystem::detectOI(int index) {
            bool ret = false ;
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            int axcnt = ds.GetStickAxisCount(index) ;

            for(int i = 0 ; i < axcnt ; i++) {
                if (std::fabs(ds.GetStickAxis(index, i)) > 0.01) {
                    ret = true ;
                    break ;
                }
            }
            return ret ;
        }
    }
}
