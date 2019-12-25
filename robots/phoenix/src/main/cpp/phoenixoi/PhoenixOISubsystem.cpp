#include "PhoenixOISubsystem.h"
#include "Phoenix.h"
#include "phoenixsubsystem/PhoenixRobotSubsystem.h"
#include <Robot.h>
#include <actions/SequenceAction.h>
#include <frc/DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixOISubsystem::PhoenixOISubsystem(Subsystem *parent) : OISubsystem(parent, "phoenixoi") {
            int oi = getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;
            auto oidev = std::make_shared<PhoenixOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        PhoenixOISubsystem::~PhoenixOISubsystem() {
        }
    }
}
