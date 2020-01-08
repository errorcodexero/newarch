#include "MotorEncoderVelocityAction.h"
#include <MessageLogger.h>
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        MotorEncoderVelocityAction::MotorEncoderVelocityAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem), target_(target) {
        }

        void MotorEncoderVelocityAction::start() {
            MotorEncoderSubsystemAction::start();

            auto &sub = getSubsystem();

            pid_.initFromSettingsExtended(
                sub.getRobot().getSettingsParser(),
                sub.getName() + ":velocity",
                sub.isAngular()
            );

            pid_.reset();
        }

        void MotorEncoderVelocityAction::run() {
            MotorEncoderSubsystemAction::run();

            auto &sub = getSubsystem();
            auto &logger = sub.getRobot().getMessageLogger() ;

            // Compute motor output.
            double out = pid_.getOutput(target_, sub.getSpeedometer().getVelocity(), sub.getRobot().getDeltaTime()) ; 
            sub.setMotor(out) ;

            logger.startMessage(MessageLogger::MessageType::debug, sub.getMsgID()) ;
            logger << "MotorEncoderVelocityAction (" << sub.getName() << "):" ;
            logger << " target " << target_ ;
            logger << " actual " << sub.getPosition() ;
            logger << " output " << out ;
            logger.endMessage() ;
        }

        void MotorEncoderVelocityAction::cancel()
        {
            MotorEncoderSubsystemAction::cancel();
            setDone();
            getSubsystem().setMotor(0.0);
        }
    }
}