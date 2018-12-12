#include "BunnyOISubsystem.h"
#include "BunnyOIDevice.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <DriverGamepad.h>

using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
        BunnyOISubsystem::BunnyOISubsystem(xero::base::Robot &robot) : OISubsystem(robot, "oi") {
            int oi = robot.getSettingsParser().getInteger("hw:driverstation:hid:oi") ;			
			auto oidev = std::make_shared<BunnyOIDevice>(*this, oi) ;
			addHIDDevice(oidev) ;
        }

        BunnyOISubsystem::~BunnyOISubsystem() {
        }
    }
}
