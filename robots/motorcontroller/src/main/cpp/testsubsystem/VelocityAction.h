#pragma once

#include <motorencodersubsystem/MotorEncoderVelocityAction.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/shuffleboard/Shuffleboard.h>

namespace xero {
    namespace motorcontroller {
        class TestSubsystem ;

        class VelocityAction : public xero::base::MotorEncoderVelocityAction {
        public:
            VelocityAction(TestSubsystem &sub) ;
            virtual ~VelocityAction() ;

            virtual void run() override ;

        private:
            TestSubsystem &sub_;
            frc::SimpleWidget widget_;
        };
    }
}