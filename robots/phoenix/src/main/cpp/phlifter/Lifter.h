#pragma once

#include <Subsystem.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <LoopType.h>
#include <frc/VictorSP.h>
#include <frc/Encoder.h>
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>

namespace xero {
    namespace phoenix {
        class Lifter : public xero::base::MotorEncoderSubsystem {
            friend class LifterCalibrateAction ;
            friend class LifterShiftAction ;
            friend class LifterBreakAction ;

        public:
            Lifter(xero::base::Subsystem *parent) ;
            virtual ~Lifter() ;

            virtual void computeState();
            virtual void run();
            virtual void init(xero::base::LoopType ltype);
            virtual bool canAcceptAction(xero::base::ActionPtr action);

        private:            
            void setLowGear() ;
            void setHighGear() ;
            void setBrakeOn() ;
            void setBrakeOff() ;

        private:
            std::shared_ptr<frc::DigitalInput> bottom_limit_ ;
            std::shared_ptr<frc::DigitalInput> top_limit_ ;
            std::shared_ptr<frc::Solenoid> gear_box_ ;
            std::shared_ptr<frc::Solenoid> brake_ ;

            bool is_calibrated_;
            bool high_gear_;
            bool brake_on_ ;

            bool bottom_limit_hit_;
            bool top_limit_hit_;

            double floor_;
        };
    }
}