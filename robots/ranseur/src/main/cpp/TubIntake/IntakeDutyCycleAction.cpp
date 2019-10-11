#include "IntakeDutyCycleAction.h"
#include "Intake.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace ranseur {
            IntakeDutyCycleAction::IntakeDutyCycleAction(Intake &intake, double v) : IntakeAction(intake) {
                duty_cycle_ = v ;
            }

            IntakeDutyCycleAction::IntakeDutyCycleAction(Intake &intake, const std::string &name) : IntakeAction(intake) {
                duty_cycle_ = intake.getRobot().getSettingsParser().getDouble(name) ;
            }

            IntakeDutyCycleAction::~IntakeDutyCycleAction() {

            }
            void IntakeDutyCycleAction::start() {
                getIntake().intakeWheels->set(duty_cycle_) ;
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
