#include "TargetTracker.h"
#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "limelight/DroidLimeLight.h"
#include "turret/Turret.h"

#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base;
using namespace xero::misc;

namespace xero {
    namespace droid {
        TargetTracker::TargetTracker(Subsystem *parent): Subsystem(parent, "TargetTracker") {
            lastCameraSampleTime_ = 0;
            source_ = Source::DriveBase;
            relativeAngle_ = 0;
            distance_ = 0;
            cameraTrackingEnabled_ = true;

            auto &settings = getRobot().getSettingsParser();
            targetAbsX_ = settings.getDouble("targettracker:target:x");
            targetAbsY_ = settings.getDouble("targettracker:target:y");
            maxCameraTrackingAngle_ = settings.getDouble("targettracker:camera_targetting_max_angle");
            cameraOffsetAngle_ = settings.getDouble("targettracker:camera_offset_angle");
        }

        void TargetTracker::setCameraTrackingEnabled(bool enabled) {
            cameraTrackingEnabled_ = enabled;
            auto limelight = static_cast<Droid&>(getRobot()).getDroidSubsystem()->getLimeLight();
            if (enabled) {
                limelight->setLedMode(LimeLight::ledMode::UseLED);
            } else {
                limelight->setLedMode(LimeLight::ledMode::ForceOff);
            }
        }

        void TargetTracker::computeState() {
            auto droid = static_cast<Droid&>(getRobot()).getDroidSubsystem();
            auto limelight = droid->getLimeLight();
            auto db = droid->getTankDrive();
            auto turret = droid->getTurret();
            auto kinematics = db->getKinematics();

            // Use the drivebase to estimate the angle towards the target.
            // TODO: I think math is right, but we should experimentally verify
            double offsetX = kinematics->getX() - targetAbsX_;
            double offsetY = kinematics->getY() - targetAbsY_;
            double bearingToTarget = atan2(offsetY, -offsetX);  // 0 degrees = away from target wall
            double dbRelAngleToTarget = bearingToTarget + turret->getPosition();
            double dbDistanceToTarget = sqrt(offsetX*offsetX + offsetY*offsetY);
            
            frc::SmartDashboard::PutNumber("DBTOffsX", offsetX);
            frc::SmartDashboard::PutNumber("DBTOffsY", offsetY);
            frc::SmartDashboard::PutNumber("DBTRelAngle", dbRelAngleToTarget);
            frc::SmartDashboard::PutNumber("DBTDist", dbDistanceToTarget);

            if (cameraTrackingEnabled_ && /*abs(dbRelAngleToTarget) < maxCameraTrackingAngle_ &&*/
                limelight->getSampleTime() > lastCameraSampleTime_) {

                // We have a new sample from the camera.
                lastCameraSampleTime_ = limelight->getSampleTime();
                source_ = Source::Camera;
                distance_ = limelight->getDistance();
                relativeAngle_ = limelight->getYaw() - cameraOffsetAngle_;
            }/* else {
                // Use a sample from the drivebase
                // TODO: we might not want to switch back and forth
                // between sample sources instantly like this
                //
                // We could use a threshold -- e.g. switch to a drivebase 
                // sample after we've lost the vision target for 100ms or so.
                // Another option that may be better in the long term is to
                // recalibrate the drivebase each time we receive a LimeLight sample.
                source_ = Source::DriveBase;
                distance_ = dbDistanceToTarget;
                relativeAngle_ = dbRelAngleToTarget;
            }*/
        }
    }
}