#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Turret;

        class Shooter : public xero::base::MotorEncoderSubsystem {
        public:
            Shooter(xero::base::Subsystem *parent);
            virtual ~Shooter() {}

            std::shared_ptr<Turret> getTurret() { return turret_; }
        private:
            std::shared_ptr<Turret> turret_;
        };
    }
}