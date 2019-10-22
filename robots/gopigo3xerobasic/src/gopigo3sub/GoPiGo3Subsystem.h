#pragma once

#include "oi/GoPiGo3OISubsystem.h"
#include <RobotSubsystem.h>
#include <tankdrive/TankDrive.h>

namespace xero {
    namespace gopigo3 {
        class GoPiGo3Subsystem : public xero::base::RobotSubsystem {
        public:
            GoPiGo3Subsystem(xero::base::Robot &robot) ;
            virtual ~GoPiGo3Subsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return tankdrive_ ;
            }

            std::shared_ptr<GoPiGo3OISubsystem> getOI() {
                return oi_ ;
            }

        private:
            std::shared_ptr<xero::base::TankDrive> tankdrive_ ;
            std::shared_ptr<GoPiGo3OISubsystem> oi_ ;
        } ;
    }
}