#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Intake : public xero::base::SingleMotorSubsystem {
        public:
            Intake(xero::base::Subsystem *parent);
            virtual ~Intake() {
            }

        private:
            std::shared_ptr<xero::base::MotorEncoderSubsystem> lifter_ ;
        };
    }
}