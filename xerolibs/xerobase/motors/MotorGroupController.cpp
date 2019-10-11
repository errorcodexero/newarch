#include "MotorGroupController.h"

namespace xero {
    namespace base {
            MotorGroupController::MotorGroupController() {}

            MotorGroupController::MotorGroupController(std::vector<MotorPtr> motors) {
                motors_ = motors;

                // Set all motors but the first to follow the first
                MotorPtr first = nullptr;
                for (MotorPtr motor : motors_) { 
                    if (first) motor->follow(first);
                    else first = motor;
                }
            }

            void MotorGroupController::add(MotorPtr motor, bool invert) {
                motors_.push_back(motor);
                if (motors_.size() > 1) motor->follow(motors_.front(), invert);
            }

            void MotorGroupController::set(double percent) {
                motors_.front()->set(percent);
            }

            void MotorGroupController::setInverted(bool inverted) {
                motor.front()->setInverted(inverted);
            }

            void MotorGroupController::setNeutralMode(NeutralMode neutralMode) {
                for (MotorPtr motor : motors_) motor->setNeutralMode(neutralMode);
            }
    }
}