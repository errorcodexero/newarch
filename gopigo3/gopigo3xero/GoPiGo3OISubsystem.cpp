#include "GoPiGo3OISubsystem.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <DriverGamepad.h>

using namespace xero::misc ;

namespace xero {
    namespace gopigo {
        GoPiGo3OISubsystem::GoPiGo3OISubsystem(xero::base::Robot &robot) : OISubsystem(robot, "oi") {
        }

        GoPiGo3OISubsystem::~GoPiGo3OISubsystem() {
        }
    }
}
