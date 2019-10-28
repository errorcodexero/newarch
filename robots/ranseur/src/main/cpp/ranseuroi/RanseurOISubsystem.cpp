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

        bool RanseurOISubsystem::detectOI(int index) {
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
