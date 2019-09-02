#include "oi/GoPiGo3OISubsystem.h"
#include "oi/GoPiGo3OIDevice.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <oi/DriverGamepad.h>

using namespace xero::misc ;

namespace xero {
    namespace gopigo3 {
        GoPiGo3OISubsystem::GoPiGo3OISubsystem(xero::base::Robot &robot) : OISubsystem(robot, "oi") {
            int oi = robot.getSettingsParser().getInteger("hw:driverstation:hid:oi");

            auto oidev = std::make_shared<GoPiGo3OIDevice>(*this, oi);
            addHIDDevice(oidev);
        }

        GoPiGo3OISubsystem::~GoPiGo3OISubsystem() {
        }
    }
}
