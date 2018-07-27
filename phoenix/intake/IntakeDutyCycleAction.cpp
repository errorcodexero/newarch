#include "IntakeDutyCycleAction.h"
#include "Intake.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {
            IntakeDutyCycleAction::IntakeDutyCycleAction(Intake &intake, double v) : IntakeAction(intake) {
                duty_cycle_ = v ;
            }
            IntakeDutyCycleAction::~IntakeDutyCycleAction() {

            }
            void IntakeDutyCycleAction::start() {
                getIntake().motor1_->Set(duty_cycle_) ;
                getIntake().motor2_->Set(duty_cycle_) ;
            }

            void IntakeDutyCycleAction::run() {
            }

            bool IntakeDutyCycleAction::isDone() {
                return true ;
            }

            void IntakeDutyCycleAction::cancel() {
                getIntake().motor1_->Set(0.0) ;
                getIntake().motor2_->Set(0.0) ;
            }

            std::string IntakeDutyCycleAction::toString() {
                std::string result = "IntakeDutyCycleAction " ;
                result += std::to_string(duty_cycle_) ;
                return result ;
            }
    }
}