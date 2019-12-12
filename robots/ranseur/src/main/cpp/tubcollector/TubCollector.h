#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <DriveBase.h>
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include <frc/VictorSP.h>
#include <frc/PowerDistributionPanel.h>
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

            void setPowerMode() {
                power_mode_ = true ;
            }

            void setSensorMode() {
                power_mode_ = false ;
            }

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

            virtual void init(xero::base::LoopType lt) ;

            double getCollectPower() {
                return collect_power_ ;
            }

            int getState() {
                return state_ ;
            }

            void setDriveBase(std::shared_ptr<xero::base::DriveBase> db) {
                db_ = db ;
            }

        private:
            //
            // This is the raw tub state, does not mean a tub is collected
            //
            bool has_tub_ ;
            std::shared_ptr<xero::misc::DebounceBoolean> debounce_ ;

            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<xero::base::MotorController> intake1_ ;
            std::shared_ptr<xero::base::MotorController> intake2_ ;            
            std::shared_ptr<frc::Solenoid> clamp_ ;

            bool power_mode_ ;
            frc::PowerDistributionPanel pdp_ ;
            double first_current_trigger_ ;
            double second_current_trigger_ ;
            double second_current_limit_ ;
            double third_current_trigger_ ;
            double start_time_ ;
            double delay_time_ ;
            double collect_power_ ;
            
            int loops_ ;
            int state_ ;
            int collector_motor_pdp_1_ ;
            int collector_motor_pdp_2_ ;

            std::shared_ptr<xero::base::DriveBase> db_ ;
        } ;
    }
}
