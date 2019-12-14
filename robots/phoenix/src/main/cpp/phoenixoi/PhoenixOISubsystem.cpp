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
#ifdef NOTYET
            int oi = getRobot().getSettingsParser().getInteger("hw:driverstation:hid:oi") ;
            auto oidev = std::make_shared<PhoenixOIDevice>(*this, oi) ;
            addHIDDevice(oidev) ;
#endif
        }

        PhoenixOISubsystem::~PhoenixOISubsystem() {
        }

        bool PhoenixOISubsystem::detectOI(int index) {
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
