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

            std::shared_ptr<xero::base::MotorController> getIntake1() {
                return intake1_ ;
            }

            std::shared_ptr<xero::base::MotorController> getIntake2() {
                return intake2_ ;
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

            void setCollectedTubState(bool st) {
                collected_tub_ = st ;
            }

            virtual void reset() ;

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
            std::shared_ptr<xero::base::MotorController> intake1_ ;
            std::shared_ptr<xero::base::MotorController> intake2_ ;            
            std::shared_ptr<frc::Solenoid> clamp_ ;
        } ;
    }
}
