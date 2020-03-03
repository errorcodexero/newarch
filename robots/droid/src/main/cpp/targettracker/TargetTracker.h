#include <Subsystem.h>

namespace xero {
    namespace droid {
        class TargetTracker : public xero::base::Subsystem {
        public:
            TargetTracker(xero::base::Subsystem *parent);

            void computeState() override;

            /// The source of a sample.
            enum class Source {
                /// This sample came directly from the LimeLight.
                Camera,

                /// No LimeLight sample was available, so this sample was
                /// obtained from the drivebase (which may have been calibrated by
                /// the LimeLight sometime in the past).
                DriveBase,
            };

            /// Returns true if the current sample is from the camera
            /// (as opposed to the drivebase).
            Source getSampleSource() { return source_; };
            
            /// Returns the time of the last LimeLight sample.
            double getLastCameraSampleTime() {
                return lastCameraSampleTime_;
            }

            /// Returns the angle from the front of the robot to the target.
            /// If we're aimed at the target, this value should be equal to
            /// the position reported by the turret encoder.
            double getDesiredTurretAngle() {
                return relativeAngle_;
            }

            /// Returns the distance to the target.
            double getDistance() {
                return distance_;
            }

            void setCameraTrackingEnabled(bool enabled);

        private:
            double lastCameraSampleTime_;
            Source source_;
            double cameraYaw_;
            double relativeAngle_;
            double distance_;

            // Config file parameters

            // The location of the target on the field.
            double targetAbsX_;
            double targetAbsY_;

            double lastAbsX_;
            double lastAbsY_;

            // The maxmimum angle to attempt camera tracking.
            double maxCameraTrackingAngle_;
            
            // This angle is subtracted from the LimeLight yaw.
            double cameraOffsetAngle_;

            bool cameraTrackingEnabled_;
        };
    }
}