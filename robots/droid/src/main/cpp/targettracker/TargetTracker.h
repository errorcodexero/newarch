#include <Subsystem.h>

namespace xero {
    namespace droid {
        class TargetTracker : public xero::base::Subsystem {
        public:
            TargetTracker(xero::base::Subsystem *parent);

            void computeState() override;
            
            /// Returns the time of the last LimeLight sample.
            double getLastCameraSampleTime() {
                return lastCameraSampleTime_;
            }

            /// Returns the angle from the turret to the target.
            double getRelativeAngle() {
                return relativeAngle_;
            }

            /// Returns the distance to the target.
            double getDistance() {
                return distance_;
            }

        private:
            double lastCameraSampleTime_;
            double relativeAngle_;
            double distance_;
        };
    }
}