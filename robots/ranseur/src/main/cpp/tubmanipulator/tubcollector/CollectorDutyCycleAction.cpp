#include "CollectorDutyCycleAction.h"
#include "Collector.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace ranseur {
            CollectorDutyCycleAction::CollectorDutyCycleAction(Collector &intake, double v) : CollectorAction(intake) {
                duty_cycle_ = v ;
            }

            CollectorDutyCycleAction::CollectorDutyCycleAction(Collector &collector, const std::string &name) : CollectorAction(collector) {
                duty_cycle_ = collector.getRobot().getSettingsParser().getDouble(name) ;
            }

            CollectorDutyCycleAction::~CollectorDutyCycleAction() {

            }
            void CollectorDutyCycleAction::start() {
                getCollector().getIntake()->set(duty_cycle_) ;
            }

            void CollectorDutyCycleAction::run() {
            }

            bool CollectorDutyCycleAction::isDone() {
                return true ;
            }

            void CollectorDutyCycleAction::cancel() {
            }

            std::string CollectorDutyCycleAction::toString() {
                std::string result = "CollectorDutyCycleAction " ;
                result += std::to_string(duty_cycle_) ;
                return result ;
            }
    }
}
