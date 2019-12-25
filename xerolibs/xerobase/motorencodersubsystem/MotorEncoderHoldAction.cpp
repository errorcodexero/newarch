#include "MotorEncoderHoldAction.h"
#include <MessageLogger.h>
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        MotorEncoderHoldAction::MotorEncoderHoldAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem), target_(target), hasExplicitTarget_(true) {
        }

        void MotorEncoderHoldAction::start() {
            auto &sub = getSubsystem();

            pid_.initFromSettingsExtended(
                sub.getRobot().getSettingsParser(),
                sub.getName() + ":hold",
                sub.isAngular()
            );

            if (!hasExplicitTarget_) target_ = getSubsystem().getPosition();
            cancelled_ = false;
            pid_.reset();
        }

        void MotorEncoderHoldAction::run() {
            auto &sub = getSubsystem();
            auto &logger = sub.getRobot().getMessageLogger() ;

            double out = pid_.getOutput(target_, sub.getPosition(), sub.getRobot().getDeltaTime()) ; 
            sub.setMotor(out) ;

            logger.startMessage(MessageLogger::MessageType::debug, sub.getMsgID()) ;
            logger << "MotorEncoderHoldAction (" << sub.getName() << "):" ;
            logger << " target " << target_ ;
            logger << " actual " << sub.getPosition() ;
            logger << " output " << out ;
            logger.endMessage() ;
        }
    }
}