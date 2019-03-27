#pragma once

#include <Subsystem.h>
#include <frc/DigitalInput.h>
#include <frc/AnalogInput.h>
#include <frc/Solenoid.h>
#include <PIDCtrl.h>
#include <Robot.h>
#include <DebounceBoolean.h>
#include <limits>
#include <vector>

namespace xero {
    namespace phaser {
        class CarlosHatch : public xero::base::Subsystem {
        public:
            CarlosHatch(xero::base::Robot &robot) ;
            virtual ~CarlosHatch() ;
            
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;
            virtual void run() ;

            void extendArm() ;
            void retractArm() ;
            void stopArm() ;

            void enableHooks() ;
            void disableHooks() ;

            bool isImpacting()  {
                return has_impact_debounced_->get() ;
            }

            bool hasHatch() {
                return has_hatch_debounced_->get() ;
            }

            bool isArmDeployed() {
                return arm_deployed_ ;
            }

            bool isHolderDeployed() {
                return holder_deployed_ ;
            }

        private:
            bool arm_deployed_ ;
            bool holder_deployed_ ;

            std::shared_ptr<frc::AnalogInput> sensor_ ;
            std::shared_ptr<frc::DigitalInput> impact_ ;
            std::shared_ptr<frc::Solenoid> arm_extend_ ;
            std::shared_ptr<frc::Solenoid> arm_retract_ ;
            std::shared_ptr<frc::Solenoid> holder_ ;

            std::shared_ptr<xero::misc::DebounceBoolean> has_impact_debounced_ ;
            std::shared_ptr<xero::misc::DebounceBoolean> has_hatch_debounced_ ;
            double hatch_present_threshold_ ;

            bool has_hatch_ ;
            bool has_impact_ ;
        } ;
    }
}
