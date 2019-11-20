#pragma once

#include <PIDCtrl.h>
#include <xeromath.h>

#include "MotorEncoderSubsystemAction.h"
#include "MotorEncoderSubsystem.h"

namespace xero {
    namespace base {
        /// An action which holds a MotorEncoderSubsystem at a single position
        /// and actively corrects for variation. Runs indefinitely until canceled.
        class MotorEncoderHoldAction: MotorEncoderSubsystemAction {
            /// Creates a hold action targeting the current position.
            MotorEncoderHoldAction(MotorEncoderSubsystem &subsystem):
                MotorEncoderHoldAction(subsystem, subsystem.getPosition()) {}

            /// Creates a hold action targeting the specified position.
            MotorEncoderHoldAction(MotorEncoderSubsystem &subsystem, double target);

            virtual void start() {}
            virtual void run();
            virtual bool isDone() { return cancelled_; }
            virtual void cancel() { cancelled_ = true; }

            /// Returns the target of this hold action.
            double getTarget() { return target_; }

            virtual std::string toString() { return "MotorEncoderHoldAction: " + std::to_string(target_); }
        private:
            bool cancelled_;
            xero::misc::PIDCtrl pid_ ;
            double target_;

            double normalizePosition(double pos) {
                if (getSubsystem().isAngular()) return xero::math::normalizeAngleDegrees(pos);
                else return pos;
            }
        };
    }
}