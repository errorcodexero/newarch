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
            void IntakeDutyCycleAction::end() {

            }
            void IntakeDutyCycleAction::run() {

            }
            bool IntakeDutyCycleAction::isDone() {

            }
            bool IntakeDutyCycleAction::cancel() {

            }
            std::string IntakeDutyCycleAction::toString(){

            }
    }
}