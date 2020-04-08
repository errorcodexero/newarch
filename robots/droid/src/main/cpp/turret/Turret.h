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

            double getMinSafeAngle() { return minSafeAngle_; }
            double getMaxSafeAngle() { return maxSafeAngle_; }

            // Returns true if the turret is aligned for a shot.
            bool isReadyToFire() { return readyToFire_; }

            void computeMyState() override;

        protected:
            // Sets the motor power, unless doing so would take us
            // into the keepout region.
            void setMotorPower(double power);

        private:
            bool readyToFire_;

            double minSafeAngle_;
            double maxSafeAngle_;

            bool emergencyStop_;

            double safetyThresholdTime_;
            double safetyThresholdDist_;
            double safetyThresholdPower_;
            
            double safetyThresholdStartTime_;
        };
    }
}