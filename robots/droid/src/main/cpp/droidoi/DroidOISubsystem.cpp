#include "DroidOISubsystem.h"
#include "DroidOIDevice.h"
#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
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
        DroidOISubsystem::DroidOISubsystem(xero::base::Subsystem *parent) : OISubsystem(parent, "oi") {
            int oi = parent->getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  
            auto oidev = std::make_shared<DroidOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        DroidOISubsystem::~DroidOISubsystem() {
        }
    }
}
