#include "TargetTracker.h"
#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "limelight/DroidLimeLight.h"

using namespace xero::base;
using namespace xero::misc;

namespace xero {
    namespace droid {
        TargetTracker::TargetTracker(Subsystem *parent): Subsystem(parent, "TargetTracker") {
            lastCameraSampleTime_ = 0;
            relativeAngle_ = 0;
            distance_ = 0;
        }

        void TargetTracker::computeState() {
            auto droid = static_cast<Droid&>(getRobot()).getDroidSubsystem();
            auto limelight = droid->getLimeLight();

            if (limelight->getSampleTime() > lastCameraSampleTime_) {
                // We have a new sample from the camera.
                lastCameraSampleTime_ = limelight->getSampleTime();
                distance_ = limelight->getDistance();
                relativeAngle_ = limelight->getYaw();
            }
        }
    }
}