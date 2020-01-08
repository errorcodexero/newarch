#include "MotorEncoderVelocityAction.h"
#include <MessageLogger.h>
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::vector<std::string> MotorEncoderVelocityAction::plot_columns_ = {
            "time", 
            "tvel", "avel", "out"
        } ;

        MotorEncoderVelocityAction::MotorEncoderVelocityAction(MotorEncoderSubsystem &subsystem, double target):
            MotorEncoderSubsystemAction(subsystem), target_(target) {
            plotid_ = subsystem.initPlot(subsystem.getName() + "-" + toString()) ;
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
            sub.startPlot(plotid_, plot_columns_) ;
        }

        void MotorEncoderVelocityAction::run() {
            MotorEncoderSubsystemAction::run();

            auto &sub = getSubsystem();
            auto &logger = sub.getRobot().getMessageLogger() ;

            // Compute motor output.
            double avel = sub.getSpeedometer().getVelocity();
            double out = pid_.getOutput(target_, avel, sub.getRobot().getDeltaTime()) ;

            sub.setMotor(out) ;

            logger.startMessage(MessageLogger::MessageType::debug, sub.getMsgID()) ;
            logger << "MotorEncoderVelocityAction (" << sub.getName() << "):" ;
            logger << " target " << target_ ;
            logger << " actual " << sub.getPosition() ;
            logger << " output " << out ;
            logger.endMessage() ;

            sub.addPlotData(plotid_, {
                sub.getRobot().getTime(),
                target_, avel, out
            });
        }

        void MotorEncoderVelocityAction::cancel()
        {
            MotorEncoderSubsystemAction::cancel();
            setDone();
            getSubsystem().setMotor(0.0);
        }
    }
}