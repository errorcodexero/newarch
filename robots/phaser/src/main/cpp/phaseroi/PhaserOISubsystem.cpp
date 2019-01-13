#include "PhaserOISubsystem.h"
#include "PhaserOIDevice.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <DriverGamepad.h>

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
    }
}
