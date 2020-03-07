#include "TargetTracker.h"
#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "limelight/DroidLimeLight.h"
#include "turret/Turret.h"
#include "droidids.h"

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

            std::string drivebaseThresholdKey = "gamepiecemanipulator:fire:max_drivebase_velocity";
            assert(settings.isDefined(drivebaseThresholdKey) &&
                   "gamepiecemanipulator:fire:max_drivebase_velocity must be defined");
            drivebaseVelocityThreshold_ = settings.getDouble(drivebaseThresholdKey);

            locked_ = false;
            hasValidSample_ = false;
        }

        void TargetTracker::setCameraTrackingEnabled(bool enabled) {
            cameraTrackingEnabled_ = enabled;
            locked_ = false;
        }

        void TargetTracker::computeState() {
            auto droid = static_cast<Droid&>(getRobot()).getDroidSubsystem();
            auto limelight = droid->getLimeLight();
            auto turret = droid->getTurret();

            auto &logger = getRobot().getMessageLogger();

            if (locked_) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TURRET_TRACKER);
                logger << "TargetTracker: locked";
                logger.endMessage();

                if (abs(droid->getTankDrive()->getVelocity()) > drivebaseVelocityThreshold_) {
                    locked_ = false;
                }
            }
            
            if (!locked_) {
                if (cameraTrackingEnabled_ && limelight->getSampleTime() > lastCameraSampleTime_) {
                    // We have a new sample from the camera.
                    lastCameraSampleTime_ = limelight->getSampleTime();
                    source_ = Source::Camera;
                    distance_ = limelight->getDistance();

                    // Compute the desired turret angle.
                    relativeAngle_ = -(limelight->getYaw() - cameraOffsetAngle_) + turret->getPosition();
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TURRET_TRACKER);
                    logger << "TargetTracker: yaw " << limelight->getYaw() << "; distance: " << distance_;
                    logger.endMessage();
                    hasValidSample_ = true;
                    
                    if (abs(droid->getTankDrive()->getVelocity()) < drivebaseVelocityThreshold_) {
                        locked_ = true;
                    }
                } else {
                    hasValidSample_ = false;
                }
            }
        }
    }
}