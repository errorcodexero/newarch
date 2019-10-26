#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include <frc/VictorSP.h>
#include <memory>
#include <motors/MotorController.h>

namespace xero {
    namespace ranseur {
        class Intake ;

        class Collector : public xero::base::Subsystem {
            friend class CollectTubAction ;
        public:
            Collector(xero::base::Subsystem* parent) ;
            virtual ~Collector() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

            std::shared_ptr<xero::base::MotorController> getIntake() {
                return intake_ ;
            }

            bool hasTub() {
                return has_tub_ ;
            }

            bool collectedTub() {
                return collected_tub_ ;
            }

            void openHand() {
                clamp_->Set(true) ;              ///true and false of the Hand subject to change/switch
            }
            void closeHand() {
                clamp_->Set(false) ;
            }

            void createNamedSequences() {
            }

     //   private:
            void setCollectedTubState(bool st) {
                collected_tub_ = st ;
            }

        private:
            //
            // This is the raw tub state, does not mean a tub is collected
            //
            bool has_tub_ ;

            //
            // This means we have collected a tub
            //
            bool collected_tub_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<xero::misc::DebounceBoolean> deb_sensor_ ;
            std::shared_ptr<xero::base::MotorController> intake_ ;
            std::shared_ptr<frc::Solenoid> clamp_ ;
        } ;
    }
}
