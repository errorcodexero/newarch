#include <Robot.h>

#include "MotorEncoderSubsystem.h"
#include "MotorEncoderSubsystemAction.h"
#include "MotorEncoderHoldAction.h"
#include "XeroEncoder.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc;

namespace xero {
    namespace base {
        MotorEncoderSubsystem::MotorEncoderSubsystem(
            Subsystem *parent, 
            const std::string &name,
            const std::string config,
            uint64_t id,
            bool angular
        ): SingleMotorSubsystem(parent, name, "hw:" + config + ":motor", id), 
        speedometer_(/*samples=*/2, angular), configName_(name), msg_id_(id) {
            auto &robot = getRobot(); 
            encoder_ = std::make_shared<XeroEncoder>(robot,
                                                     "hw:" + config + ":encoder",
                                                     angular);

        }

        void MotorEncoderSubsystem::postHWInit()
        {
            SingleMotorSubsystem::postHWInit() ;
            setDefaultAction(std::make_shared<MotorEncoderHoldAction>(*this));
        }

        bool MotorEncoderSubsystem::canAcceptAction(xero::base::ActionPtr action) {
            if (SingleMotorSubsystem::canAcceptAction(action))
                return true ;

            auto mot_enc_act_p = std::dynamic_pointer_cast<MotorEncoderSubsystemAction>(action);
            return (mot_enc_act_p != nullptr);
        }

        bool MotorEncoderSubsystem::canAcceptDefaultAction(xero::base::ActionPtr action) { 
            return std::dynamic_pointer_cast<MotorEncoderHoldAction>(action) != nullptr;
        }

        void MotorEncoderSubsystem::computeState() {
            SingleMotorSubsystem::computeState();

            double pos = encoder_->getPosition() ;
            
            speedometer_.update(getRobot().getDeltaTime(), pos) ;
            if (smartDashboardName_.length() > 0)
                frc::SmartDashboard::PutNumber(smartDashboardName_, pos) ;

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(xero::misc::MessageLogger::MessageType::debug, msg_id_) ;
            logger << getName() << ":" ;
            logger << " pos " << pos ;
            logger << " vel " << speedometer_.getVelocity() ;
            logger << " accel " << speedometer_.getAcceleration() ;
            logger.endMessage() ;
        }

        void MotorEncoderSubsystem::reset() {
            SingleMotorSubsystem::reset() ;
        }
    }
}