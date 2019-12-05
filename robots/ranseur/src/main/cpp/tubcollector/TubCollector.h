#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include <frc/VictorSP.h>
#include <memory>
#include <motors/MotorController.h>

/// need to rename everything with "tub" in front ///

namespace xero {
    namespace ranseur {
        class Intake ;

        class TubCollector : public xero::base::Subsystem {
            friend class TubCollectTubAction ;
        public:
            TubCollector(xero::base::Subsystem* parent) ;
            virtual ~TubCollector() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

            void setIntakePower(double pow) {
                intake1_->set(pow) ;
                intake2_->set(pow) ;
            }

            bool hasTub() {
                return has_tub_ ;
            }

            void openHand() {
                clamp_->Set(true) ;              ///true and false of the Hand subject to change/switch
            }
            void closeHand() {
                clamp_->Set(false) ;
            }

            void createNamedSequences() {
            }

            virtual void reset() ;

        private:
            //
            // This is the raw tub state, does not mean a tub is collected
            //
            bool has_tub_ ;

            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<xero::base::MotorController> intake1_ ;
            std::shared_ptr<xero::base::MotorController> intake2_ ;            
            std::shared_ptr<frc::Solenoid> clamp_ ;
        } ;
    }
}
