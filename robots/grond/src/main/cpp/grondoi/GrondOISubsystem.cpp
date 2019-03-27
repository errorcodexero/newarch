#include "GrondOISubsystem.h"
#include "GrondOIDevice.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <oi/DriverGamepad.h>

using namespace xero::misc ;

namespace xero {
    namespace grond {
        GrondOISubsystem::GrondOISubsystem(xero::base::Robot &robot) : OISubsystem(robot, "oi") {
            int oi = robot.getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  

            auto oidev = std::make_shared<GrondOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        GrondOISubsystem::~GrondOISubsystem() {
        }
    }
}
