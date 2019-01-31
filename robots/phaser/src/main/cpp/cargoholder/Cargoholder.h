#pragma once

#include <Subsystem.h>
#include <ctre/Phoenix.h>
#include <frc/DigitalInput.h>
#include <PIDCtrl.h>
#include <limits>
#include <vector>

typedef ctre::phoenix::motorcontrol::can::VictorSPX VictorSPX;

namespace xero {
    namespace phaser {
        class CargoHolder : public xero::base::Subsystem {
        public:
            CargoHolder(xero::base::Robot &robot) ;
            virtual ~CargoHolder() ;
            
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;

            bool hasCargo() const {
                return has_cargo_ ;
            }

            void setMotor(double power) {
                motor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, power) ;
            }


        private:
            bool has_cargo_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<VictorSPX> motor_ ;            
        } ;
    }
}
