#include "MotorControllerOISubsystem.h"
#include "MotorControllerOIDevice.h"
#include "MotorController.h"
#include "motorcontrollersubsystem/MotorControllerSubsystem.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <oi/DriverGamepad.h>
#include <actions/SequenceAction.h>
#include <frc/DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {
        MotorControllerOISubsystem::MotorControllerOISubsystem(xero::base::Subsystem *parent) : OISubsystem(parent, "oi") {
            int oi = parent->getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  
            auto oidev = std::make_shared<MotorControllerOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        MotorControllerOISubsystem::~MotorControllerOISubsystem() {
        }
    }
}
