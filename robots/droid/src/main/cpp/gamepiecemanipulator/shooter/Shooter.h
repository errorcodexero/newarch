#pragma once

#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Turret;

        class Shooter : public xero::base::MotorEncoderSubsystem {

        public:

            Shooter(xero::base::Subsystem *parent);
            virtual ~Shooter() {}

            void setReadyToFire(bool ready_to_fire) {
                ready_to_fire_ = ready_to_fire;
            }

            bool isReadyToFire() const {
                return ready_to_fire_;
            }

        private:

            bool ready_to_fire_ ;      // Set to true when shooter at the required shooting velocity
        };
    }
}