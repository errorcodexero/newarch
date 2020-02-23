#pragma once
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

#include "Climber.h"

namespace xero {
    namespace droid {
        class Lifter: public xero::base::MotorEncoderSubsystem {
        public:
            Lifter(Climber *parent, const std::string &name, uint64_t id, int samples = 2, bool angular = false ):
                MotorEncoderSubsystem(parent, name, id, samples, angular), climber_(parent) {}

        protected:
            void setMotor(double power) override {
                if (power < 0) {
                    if (climber_->isInFieldMode()) {
                        // Don't allow us to cross the limit.
                        if (getPosition() < 0) power = 0;
                        // If we're below 1000, limit power to 10%.
                        else if (getPosition() < 1000 && power < -0.1) power = -0.1;
                    } else {
                        // In pit mode, restrict to 10% always (we can't assume we have working encoders)
                        if (power < -0.1) power = -0.1;
                    }
                } else if (power > 0) {
                    // If we're above the max height, don't allow movement at all. 
                    if (getPosition() > climber_->getMaxHeight()) power = 0;
                }

                MotorEncoderSubsystem::setMotor(power);
            }

        private:
            Climber *climber_;
        };
    }
}