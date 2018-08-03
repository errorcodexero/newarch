#include "IntakeDutyCycleAction.h"
#include "Intake.h"
#include <cassert>

using namespace xero::base;

namespace xero {
    namespace phoenix {
            IntakeDutyCycleAction::IntakeDutyCycleAction(Intake &intake, double v) : IntakeAction(intake) {
                duty_cycle_ = v ;
            }

            IntakeDutyCycleAction::IntakeDutyCycleAction(Intake &intake, const std::string &name) : IntakeAction(intake) {
                //
                // Add implementation for parameter file when the parameter file is available
                //
                assert(false) ;
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
            }

            std::string IntakeDutyCycleAction::toString() {
                std::string result = "IntakeDutyCycleAction " ;
                result += std::to_string(duty_cycle_) ;
                return result ;
            }
    }
}