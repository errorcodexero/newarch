#include "RanseurOISubsystem.h"
#include "RanseurOIDevice.h"
#include "Ranseur.h"
#include "ranseurrobotsubsystem/RanseurRobotSubsystem.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <oi/DriverGamepad.h>
#include <actions/SequenceAction.h>
#include <frc/DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        RanseurOISubsystem::RanseurOISubsystem(xero::base::Subsystem *parent) : OISubsystem(parent, "oi") {
            int oi = getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  

            auto oidev = std::make_shared<RanseurOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;

            
        }

        RanseurOISubsystem::~RanseurOISubsystem() {
        }
    }
}
