#pragma once

#include "grondoi/GrondOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include <MessageListener/MessageListener.h>

namespace xero {
    namespace grond {

        class GrondSubsystem : public xero::base::RobotSubsystem {
        public:
            GrondSubsystem(xero::base::Robot &robot) ;
            virtual ~GrondSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }

            std::shared_ptr<GrondOISubsystem> getOI() {
                return oi_ ;
            }    
        private:
            std::shared_ptr<GrondOISubsystem> oi_ ;
        } ;
    }
}