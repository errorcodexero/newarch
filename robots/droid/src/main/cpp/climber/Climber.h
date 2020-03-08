#pragma once

#include <Subsystem.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <frc/Servo.h>
#include <rev/SparkMax.h>

namespace xero {
    namespace droid {
        class Climber : public xero::base::Subsystem {
            friend class ClimberTraverseAction ;
            friend class CalibrateClimberAction ;
            
        public:
            Climber(xero::base::Subsystem *parent);
            virtual ~Climber() {}

            virtual void run() ;
            
            std::shared_ptr<xero::base::MotorEncoderSubsystem> getLifter() {
                return lifter_ ;
            }

            void setTraverserPower(double p) {
                traverser_->Set(p) ;
            }

            double getMaxHeight() {
                return max_height_ ;
            }

            void init(xero::base::LoopType ltype);

            /// Returns true if the robot is on the field (as opposed to in the pits.)
            /// In pit mode, the downwards position limit is disabled and downwards
            /// speed is capped at 10%.
            ///
            /// The climber is in field mode if the robot is connected to the FMS
            /// or if the climber:force_field_mode parameter is set to true.
            bool isInFieldMode() { return field_mode_; }

            bool canAcceptAction(xero::base::ActionPtr action) override;

            bool canAcceptDefaultAction(xero::base::ActionPtr action) override
            {
                return canAcceptAction(action) ;
            }

        private:
            std::shared_ptr<xero::base::MotorEncoderSubsystem> lifter_;
            std::shared_ptr<rev::SparkMax> traverser_ ;
            uint64_t msg_id_;
            double max_height_ ;
            bool field_mode_;
            bool calibrated_ ;
        };
    }
}

//servo : hitec hs-422
