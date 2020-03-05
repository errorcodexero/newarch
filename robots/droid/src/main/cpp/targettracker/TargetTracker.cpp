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

            sampleIndex_ = 0;
            hasValidSample_ = false;
            for (auto &sample : samples_) {
                sample = std::nullopt;
            }
        }

        void TargetTracker::setCameraTrackingEnabled(bool enabled) {
            cameraTrackingEnabled_ = enabled;
        }

        void TargetTracker::computeState() {
            auto droid = static_cast<Droid&>(getRobot()).getDroidSubsystem();
            auto limelight = droid->getLimeLight();
            auto turret = droid->getTurret();

            auto &logger = getRobot().getMessageLogger();

            if (cameraTrackingEnabled_ && /*abs(dbRelAngleToTarget) < maxCameraTrackingAngle_ &&*/
                limelight->getSampleTime() > lastCameraSampleTime_) {

                // We have a new sample from the camera.
                lastCameraSampleTime_ = limelight->getSampleTime();
                source_ = Source::Camera;
                double distance = limelight->getDistance();

                // Compute the desired turret angle.
                double relativeAngle = -(limelight->getYaw() - cameraOffsetAngle_) + turret->getPosition();
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TURRET_TRACKER);
                logger << "TargetTracker: yaw " << limelight->getYaw();
                logger.endMessage();

                samples_[sampleIndex_] = { relativeAngle, distance };
            } else {
                samples_[sampleIndex_] = std::nullopt;
                // Use a sample from the drivebase
                // TODO: we might not want to switch back and forth
                // between sample sources instantly like this
                //
                // We could use a threshold -- e.g. switch to a drivebase 
                // sample after we've lost the vision target for 100ms or so.
                // Another option that may be better in the long term is to
                // recalibrate the drivebase each time we receive a LimeLight sample.
                /*
                source_ = Source::DriveBase;
                distance_ = dbDistanceToTarget;
                relativeAngle_ = dbRelAngleToTarget;
                */
            }

            sampleIndex_ += 1;
            sampleIndex_ %= samples_.size();

            if (sampleIndex_ == 0) {

                // Only trust the limelight if it saw the target 60% of the time.
                // Then, take the average, throwing out the high and low samples.
                std::optional<double> highAngle = std::nullopt;
                std::optional<double> lowAngle = std::nullopt;
                std::optional<double> highDist = std::nullopt;
                std::optional<double> lowDist = std::nullopt;
                double angleSum = 0;
                double distSum = 0;
                int valid = 0;
                for (auto sample : samples_) {
                    if (!sample) continue;

                    double angle = sample->desiredTurretAngle;
                    double dist = sample->distance;

                    valid++;
                    
                    // Check if this sample is a high or low.
                    bool angleOutlier = false;
                    if (!highAngle || angle > highAngle) {
                        // If we previously had a high angle, it's not the high angle anymore. Keep it.
                        if (highAngle) angleSum += *highAngle;
                        highAngle = angle;
                        angleOutlier = true;
                    }
                    if (!lowAngle || angle < lowAngle) {
                        if (lowAngle) angleSum += *lowAngle;
                        lowAngle = angle;
                        angleOutlier = true;
                    }
                    // If it's not the high or low, keep it.
                    if (!angleOutlier) angleSum += angle;

                    // Do the same calculation, but for distance.
                    bool distOutlier = false;
                    if (!highDist || dist > highDist) {
                        if (highDist) distSum += *highDist;
                        highDist = dist;
                        distOutlier = true;
                    }
                    if (!lowDist || dist < lowDist) {
                        if (lowDist) distSum += *lowDist;
                        lowDist = dist;
                        distOutlier = true;
                    }
                    if (!distOutlier) distSum += dist;
                }

                if (valid > 5) {
                    hasValidSample_ = true;

                    // Set the result distance and angle to the average excluding outliers.
                    distance_ = distSum/(valid - 2);
                    relativeAngle_ = angleSum/(valid - 2);

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TURRET_TRACKER);
                    logger << "TargetTracker: has valid samples (count: " << valid << ", average: " << distance_ << "): ";
                    for (auto sample : samples_) {
                        if (sample) logger << sample->distance << ",";
                        else logger << "null,";
                    }
                    logger.endMessage();
                } else {
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TURRET_TRACKER);
                    logger << "TargetTracker: does not have valid samples (count: " << valid << ") ";
                    for (auto sample : samples_) {
                        if (sample) logger << sample->distance << ",";
                        else logger << "null,";
                    }
                    logger.endMessage();
                    hasValidSample_ = false;
                }
            }
        }
    }
}