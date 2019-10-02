#pragma once

#include <Subsystem.h>
#include <ctre/Phoenix.h>
#include <frc/DigitalInput.h>
#include <PIDCtrl.h>
#include <limits>
#include <vector>
#include "singlemotorsubsystem/SingleMotorSubsystem.h"


typedef ctre::phoenix::motorcontrol::can::VictorSPX VictorSPX;

namespace xero {
    namespace phaser {
        class CargoHolder : public xero::base::SingleMotorSubsystem {
        public:
            CargoHolder(xero::base::Robot &robot, uint64_t id) ;
            virtual ~CargoHolder() ;
            
            virtual void computeState() ;

            bool hasCargo() const {
                return has_cargo_ ;
            }

        private:
            bool has_cargo_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
        } ;
    }
}
