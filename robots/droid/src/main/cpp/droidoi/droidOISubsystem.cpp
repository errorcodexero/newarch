#include "droidOISubsystem.h"
#include "droidOIDevice.h"
#include "droid.h"
#include "droidsubsystem/droidSubsystem.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <oi/DriverGamepad.h>
#include <actions/SequenceAction.h>
#include <frc/DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace droid {
        droidOISubsystem::droidOISubsystem(xero::base::Subsystem *parent) : OISubsystem(parent, "oi") {
            int oi = parent->getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  
            auto oidev = std::make_shared<droidOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        droidOISubsystem::~droidOISubsystem() {
        }
    }
}
