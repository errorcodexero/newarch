#include "$$RobotName$$OISubsystem.h"
#include "$$RobotName$$OIDevice.h"
#include "$$RobotName$$.h"
#include "$$robotname$$subsystem/$$RobotName$$Subsystem.h"
#include <Robot.h>
#include <SettingsParser.h>
#include <oi/DriverGamepad.h>
#include <actions/SequenceAction.h>
#include <frc/DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace $$robotname$$ {
        $$RobotName$$OISubsystem::$$RobotName$$OISubsystem(xero::base::Subsystem *parent) : OISubsystem(parent, "oi") {
            int oi = parent->getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;  

            auto oidev = std::make_shared<$$RobotName$$OIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
        }

        $$RobotName$$OISubsystem::~$$RobotName$$OISubsystem() {
        }

        bool $$RobotName$$OISubsystem::detectOI(int index) {
            bool ret = false ;
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;
            int axcnt = ds.GetStickAxisCount(index) ;

            for(int i = 0 ; i < axcnt ; i++) {
                if (std::fabs(ds.GetStickAxis(index, i)) > 0.01) {
                    ret = true ;
                    break ;
                }
            }
            return ret ;
        }
    }
}
