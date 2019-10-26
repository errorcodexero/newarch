#include <Robot.h>

#include "MotorEncoderSubsystem.h"
#include "MotorEncoderSubsystemAction.h"

namespace xero {
    namespace base {
        MotorEncoderSubsystem::MotorEncoderSubsystem(
            Subsystem *parent, 
            const std::string &name, 
            const std::string config,
            uint64_t id
        ): SingleMotorSubsystem(parent, name, config + ":motor", id), configName_(name), msg_id_(id) {
            
            auto &settings = getRobot().getSettingsParser();
            encoder_ = std::make_shared<frc::Encoder>(
                settings.getInteger(config + ":encoder:1"),
                settings.getInteger(config + ":encoder:2")
            );

            unitsPerTick_ = settings.getDouble(config + ":encoder:units_per_tick");
            zeroTicks_ = settings.getInteger(config + ":encoder:zero_ticks");
        }

        double MotorEncoderSubsystem::ticksToUnits(int ticks) {
            return (ticks - zeroTicks_) * unitsPerTick_;
        }

        bool MotorEncoderSubsystem::canAcceptAction(xero::base::ActionPtr action) {
            return std::dynamic_pointer_cast<MotorEncoderSubsystemAction>(action) != nullptr;
        }

        void MotorEncoderSubsystem::computeState() {
            encoderValue_ = encoder_->Get();
            position_ = ticksToUnits(encoderValue_);
        }
    }
}