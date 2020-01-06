#include <Subsystem.h>
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

            virtual void computeState();

        private:
            std::shared_ptr<frc::AnalogInput> ainput_;
            double avalue_;
        };
    }
}