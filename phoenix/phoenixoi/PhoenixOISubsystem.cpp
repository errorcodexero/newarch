#include "PhoenixOISubsystem.h"
#include "Phoenix.h"
#include "phoenixsubsystem/PhoenixRobotSubsystem.h"
#include <Robot.h>
#include <ActionSequence.h>
#include <DriverStation.h>
#include <iostream>
#include <cmath>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixOISubsystem::PhoenixOISubsystem(Robot &robot) : OISubsystem(robot, "phoenixoi") {
            int oi = robot.getSettingsParser().getInteger("hw:driverstation:hid:oi") ;
            int gunner = robot.getSettingsParser().getInteger("hw:driverstation:hid:gunner") ;

<<<<<<< HEAD
            auto oidev = std::make_shared<PhoenixOIDevice>(*this, oi) ;
			addHIDDevice(oidev) ;
=======
            if (detectOI(oi)) {
                auto oidev = std::make_shared<PhoenixOIDevice>(*this, oi) ;
				addHIDDevice(oidev) ;
            }
            else {
                auto gunnerdev = std::make_shared<PhoenixGunnerGamepad>(*this, gunner) ;
				addHIDDevice(gunnerdev) ;
            }
>>>>>>> phoenix
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
