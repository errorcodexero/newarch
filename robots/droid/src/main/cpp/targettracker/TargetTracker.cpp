#include "TargetTracker.h"
#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "limelight/DroidLimeLight.h"
#include "turret/Turret.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include <xeromath.h>

using namespace xero::base;
using namespace xero::misc;
using namespace xero::math;

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
            //auto kinematics = db->getKinematics();

            double heading = db->getAngle();


            if (cameraTrackingEnabled_ && limelight->getSampleTime() > lastCameraSampleTime_) {
                // We have new data from the camera. Store it.

                lastCameraSampleTime_ = limelight->getSampleTime();
                source_ = Source::Camera;
                distance_ = limelight->getDistance();

                // This angle is relative to the drivebase heading.
                // It is equal to the encoder position that the turret will aim for.
                relativeAngle_ = limelight->getYaw() + turret->getPosition() - cameraOffsetAngle_;
            }

            // A diagram of the coordinate system used here is available on the wiki:
            // https://www.xerosw.org/dokuwiki/doku.php?id=robot2020:coordinatesystem
            // I think the math is right, but we should experimentally verify
            
            double absX;
            double absY;

            if (cameraTrackingEnabled_ && /*abs(dbRelAngleToTarget) < maxCameraTrackingAngle_ &&*/
                limelight->getSampleTime() > lastCameraSampleTime_) {

                // We have a new sample from the camera.
                lastCameraSampleTime_ = limelight->getSampleTime();
                source_ = Source::Camera;
                double distance = limelight->getDistance();

                // This angle is relative to the drivebase heading.
                // It is equal to the encoder position that the turret will aim for.
                double relativeAngle = limelight->getYaw() + turret->getPosition() - cameraOffsetAngle_;


                // Compute our absolute position on the field. In the future, we'll need this for
                // aiming at the inner port and for shooting on the move.
                
                double bearingToTarget = relativeAngle - heading;    // The absolute angle to the target.
                
                // Compute the offsets from our position to the target.
                double offsetX = cos(deg2rad(bearingToTarget)) * distance; // Always negative, since
                                                                           // the target is to the "west."
                double offsetY = sin(deg2rad(bearingToTarget)) * distance; // Probably positive, since the
                                                                           // target is usually to the "north."

                absX = targetAbsX_ - offsetX;
                absY = targetAbsY_ - offsetY;
                lastAbsX_ = absX;
                lastAbsY_ = absY;

                // Consider updating kinematics, since we have an absX and absY from vision
            } else {
                // Future: use the drivebase to estimate the angle towards the target.
                // We don't use this data currently, but we will
                // use it later in order to e.g. shoot on the move

                // We might not want to switch back and forth
                // between sample sources instantly like this.
                //
                // We could use a threshold -- e.g. switch to a drivebase 
                // sample after we've lost the vision target for 100ms or so.
                // Another option that may be better in the long term is to
                // recalibrate the drivebase each time we receive a LimeLight sample.

                /*
                source_ = Source::DriveBase;
                absX = kinematics->getX();
                absY = kinematics->getY();
                */

               // For now, just use the last LL sample.
               absX = lastAbsX_;
               absY = lastAbsY_;
            }

            // Aim using the absolute position calculated above.
            // For now, this will effectively undo the calculations to get back to bearing/distance.
            // However, this math can be easily modified to support drivebase aiming/inner goal aiming.

            // Get the offset from our position to the target.
            double offsetX = absX - targetAbsX_;
            double offsetY = absY - targetAbsY_;

            // If we want to aim for the inner port, we just have to change targetAbsX_.
            // We should also check to be sure we won't hit the side of the target:
            // double slope = offsetY/offsetX
            // if (abs(slope * (innerPortAbsX_ - targetAbsX_)) > outerPortWidth_) {
            //     we'll hit the side, aim for the outer port instead of the inner port
            // }

            double bearingToTarget = rad2deg(atan2(offsetY, -offsetX));  // 0 degrees = away from target wall
            relativeAngle_ = bearingToTarget - heading;
            distance_ = sqrt(offsetX*offsetX + offsetY*offsetY);
        }
    }
}