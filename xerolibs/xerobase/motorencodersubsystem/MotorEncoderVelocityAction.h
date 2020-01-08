#pragma once

#include <PIDCtrl.h>
#include <xeromath.h>

#include "MotorEncoderSubsystemAction.h"
#include "MotorEncoderSubsystem.h"

namespace xero {
    namespace base {
        /// An action which holds a MotorEncoderSubsystem at a single velocity
        /// and actively corrects for variation. Runs indefinitely until canceled.
        class MotorEncoderHoldAction: public MotorEncoderSubsystemAction {
        public:
            /// Creates a velocity action targeting the specified velocity.
            MotorEncoderHoldAction(MotorEncoderSubsystem &subsystem, double target);

            virtual void start();
            virtual void run();
            virtual void cancel();

            /// Returns the target of this velocity action.
            double getTarget() { return target_; }

            virtual std::string toString() { 
                return "VelocityAction: " + std::to_string(target_);
            }
        private:
            xero::misc::PIDCtrl pid_ ;
            double target_;

            double normalizePosition(double pos) {
                if (getSubsystem().isAngular()) return xero::math::normalizeAngleDegrees(pos);
                else return pos;
            }
        };
    }
}