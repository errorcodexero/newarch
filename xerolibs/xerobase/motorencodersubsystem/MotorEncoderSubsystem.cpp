#include <Robot.h>

#include "MotorEncoderSubsystem.h"
#include "MotorEncoderSubsystemAction.h"

#include "Encoder.h"

namespace xero {
    namespace base {
        MotorEncoderSubsystem::MotorEncoderSubsystem(
            Subsystem *parent, 
            const std::string &name, 
            const std::string config,
            uint64_t id
        ): SingleMotorSubsystem(parent, name, config + ":motor", id), configName_(name), msg_id_(id) {
            
            auto &robot = getRobot(); 
            encoder_ = std::make_shared<Encoder>(robot.getMessageLogger(), robot.getSettingsParser(), config + ":encoder");
        }

        bool MotorEncoderSubsystem::canAcceptAction(xero::base::ActionPtr action) {
            if (SingleMotorSubsystem::canAcceptAction(action))
                return true ;

            auto mot_enc_act_p = std::dynamic_pointer_cast<MotorEncoderSubsystemAction>(action);
            return (mot_enc_act_p != nullptr);
        }

        void MotorEncoderSubsystem::computeState() {
            SingleMotorSubsystem::computeState();
            
            speedometer_.update(encoder_->getPosition(), getRobot().getDeltaTime());
        }

        void MotorEncoderSubsystem::reset() {
            encoder_->reset();
        }
    }
}