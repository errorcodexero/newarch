#include "FireAction.h"

#include <SettingsParser.h>

#include "Droid.h"
#include "droidsubsystem/DroidSubsystem.h"
#include "conveyor/Conveyor.h"
#include "conveyor/ConveyorEmitAction.h"
#include "shooter/Shooter.h"
#include "shooter/ShooterVelocityAction.h"
#include "targettracker/TargetTracker.h"
#include "turret/Turret.h"

using namespace xero::base;
using namespace xero::misc;
namespace xero {
    namespace droid {
        FireAction::FireAction(GamePieceManipulator &manip): 
            Action(manip.getRobot().getMessageLogger()),
            subsystem_(manip),
            droidSubsystem_(*static_cast<Droid&>(manip.getRobot()).getDroidSubsystem()) {
            
            auto &settings = manip.getRobot().getSettingsParser();
            std::string drivebaseThresholdKey = "gamepiecemanipulator:fire:max_drivebase_velocity";
            assert(settings.isDefined(drivebaseThresholdKey) &&
                   "gamepiecemanipulator:fire:max_drivebase_velocity must be defined");
            drivebaseVelocityThreshold_ = settings.getDouble(drivebaseThresholdKey);
            
            std::string cameraThresholdKey = "gamepiecemanipulator:fire:max_camera_sample_age";
            assert(settings.isDefined(cameraThresholdKey) &&
                   "gamepiecemanipulator:fire:max_camera_sample_age must be defined");
            cameraSampleAgeThreshold_ = settings.getDouble(cameraThresholdKey);
        }

        void FireAction::start() {
            isFiring_ = false;
            shooterVelocityAction_->setTarget(0);   // just set it to a known value,
            shooterVelocityAction_->start();        // we'll update it every tick
        }

        void FireAction::run() {
            auto tracker = droidSubsystem_.getTargetTracker();
            auto turret = droidSubsystem_.getTurret();
            auto drivebase = droidSubsystem_.getTankDrive();
            auto conveyor = getSubsystem().getConveyor();
            auto shooter = getSubsystem().getShooter();

            // TODO: Compute the target velocity for the shooter.
            double targetRPMs = 0;
            shooterVelocityAction_->setTarget(targetRPMs);
            
            double sampleAge = getSubsystem().getRobot().getTime() - tracker->getLastCameraSampleTime();

            bool trackerReady = sampleAge < cameraSampleAgeThreshold_;
            bool turretReady = turret->isReadyToFire();
            bool shooterReady = shooter->isReadyToFire();
            bool drivebaseReady = abs(drivebase->getVelocity()) < drivebaseVelocityThreshold_;

            bool readyToFireExceptShooter = trackerReady && turretReady && drivebaseReady;
            bool readyToFire = readyToFireExceptShooter && shooterReady;

            if (isFiring_) {
                // If we're out of balls, stop firing
                if (conveyor->isEmpty()) {
                    setDone();
                    stopChildActions();
                }

                // If we lose the taret, stop firing
                // Don't check the shooter because we expect some variance in speed as we
                // fire. The shooter should stabalize between shots. If it doesn't, then
                // we should add a delay between shots in ConveyorEmitAction.
                if (!readyToFireExceptShooter) {
                    conveyorEmitAction_->stopFiring();
                    isFiring_ = false;
                }
            } else { // We're not currently firing.
                // If we're out of balls, stop trying to fire
                if (conveyor->isEmpty()) {
                    setDone();
                    stopChildActions();
                }
                else if (readyToFire && !conveyor->isBusy()) {
                    // fire!
                    conveyor->setAction(conveyorEmitAction_);
                    isFiring_ = true;
                }
            }

            // Print a debug message
            auto &logger = getMessageLogger();
            logger.startMessage(MessageLogger::MessageType::debug);
            logger << "FireAction: isFiring: " << isFiring_ << "; ";
            if (readyToFire) logger << "ready to fire";
            else { 
                logger << "not ready to fire [";
                logger << "camera: " << (trackerReady ? "ready" : "waiting") << ", ";
                logger << "turret: " << (turretReady ? "ready" : "waiting") << ", ";
                logger << "shooter: " << (shooterReady ? "ready" : "waiting") << ", ";
                logger << "drivebase: " << (drivebaseReady ? "ready" : "waiting") << ", ";
                logger << "]";
                
            }
            logger.endMessage();
        }

        void FireAction::cancel() {
            Action::cancel();
            stopChildActions();
            setDone();
        }

        void FireAction::stopChildActions() {
            shooterVelocityAction_->cancel();
            conveyorEmitAction_->cancel();
        }
    }
}