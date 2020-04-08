#pragma once

#include "VelocityAction.h"
#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <frc/AnalogInput.h>

namespace xero
{
    namespace motorcontroller
    {
        class TestSubsystem : public xero::base::MotorEncoderSubsystem
        {
        public:
            TestSubsystem(xero::base::Subsystem* parent) ;
            virtual ~TestSubsystem() ;

            void init(xero::base::LoopType ltype) override;

            bool canAcceptAction(xero::base::ActionPtr act) override {
                return std::dynamic_pointer_cast<VelocityAction>(act) != nullptr;
            }

            void computeMyState() override;

        private:
        };
    }
}