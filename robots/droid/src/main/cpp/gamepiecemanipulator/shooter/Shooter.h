#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Turret;

        class Shooter : public xero::base::MotorEncoderSubsystem {

        public:

            Shooter(xero::base::Subsystem *parent);
            virtual ~Shooter() {}

            void setReadyToShoot(bool ready_to_shoot) {
                ready_to_shoot_ = ready_to_shoot;
            }

            bool isReadyToShoot() const {
                return ready_to_shoot_;
            }

        private:

            bool ready_to_shoot_ ;      // Set to true when shooter at the required shooting velocity
        };
    }
}