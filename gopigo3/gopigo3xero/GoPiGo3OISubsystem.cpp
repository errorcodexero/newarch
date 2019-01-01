#include "GoPiGo3OISubsystem.h"
#include "GoPiGo3OIDevice.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <DriverGamepad.h>

using namespace xero::misc ;

namespace xero {
    namespace gopigo {
        GoPiGo3OISubsystem::GoPiGo3OISubsystem(xero::base::Robot &robot) : OISubsystem(robot, "oi") {
            int oi = robot.getSettingsParser().getInteger("hw:driverstation:hid:oi");

            auto oidev = std::make_shared<GoPiGo3OIDevice>(*this, oi);
            addHIDDevice(oidev);
        }

        GoPiGo3OISubsystem::~GoPiGo3OISubsystem() {
        }
    }
}
