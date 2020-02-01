#pragma once
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace droid {
        class Turret : public xero::base::MotorEncoderSubsystem {
            friend class FollowTargetAction;
            friend class TurretStopAction;
        public:
            Turret(xero::base::Subsystem *parent);
            virtual ~Turret() {}

            void postHWInit() override;
            bool canAcceptAction(xero::base::ActionPtr action) override;
            bool canAcceptDefaultAction(xero::base::ActionPtr action) override;

            // Returns true if the turret is aligned for a shot.
            bool isReadyToFire() { return readyToFire_; }
        private:
            bool readyToFire_;
        };
    }
}