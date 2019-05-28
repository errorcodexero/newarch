#pragma once

#include "servosubsystem/ServoSubsystem.h"
#include "ledsubsystem/LEDSubsystem.h"
#include "oi/GoPiGo3OISubsystem.h"
#include <subsystem.h>
#include <tankdrive/TankDrive.h>

namespace xero {
    namespace gopigo3 {
        class GoPiGo3Subsystem : public xero::base::Subsystem {
        public:
            GoPiGo3Subsystem(xero::base::Robot &robot) ;
            virtual ~GoPiGo3Subsystem() ;

            std::shared_ptr<ServoSubsystem> getServoSubsystem() {
                return servo_ ;
            }

            std::shared_ptr<LEDSubsystem> getLEDSubsystem() {
                return led_;
            }

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return tankdrive_ ;
            }

            std::shared_ptr<GoPiGo3OISubsystem> getOI() {
                return oi_ ;
            }

        private:
            std::shared_ptr<ServoSubsystem> servo_ ;
            std::shared_ptr<LEDSubsystem> led_;
            std::shared_ptr<xero::base::TankDrive> tankdrive_ ;
            std::shared_ptr<GoPiGo3OISubsystem> oi_ ;
        } ;
    }
}