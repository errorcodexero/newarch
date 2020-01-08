#pragma once

#include <motorencodersubsystem/MotorEncoderVelocityAction.h>

namespace xero {
    namespace motorcontroller {
        class TestSubsystem ;

        class VelocityAction : public xero::base::MotorEncoderVelocityAction {
        public:
            VelocityAction(TestSubsystem &sub) ;
            virtual ~VelocityAction() ;

            void run() override ;

        private:
            TestSubsystem &sub_; 
        } ;
    }
}