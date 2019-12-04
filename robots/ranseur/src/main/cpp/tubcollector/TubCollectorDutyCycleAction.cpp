#include "TubCollectorDutyCycleAction.h"
#include "TubCollector.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace ranseur {
            TubCollectorDutyCycleAction::TubCollectorDutyCycleAction(TubCollector &intake, double v) : TubCollectorAction(intake) {
                duty_cycle_ = v ;
            }

            TubCollectorDutyCycleAction::TubCollectorDutyCycleAction(TubCollector &tubcollector, const std::string &name) : TubCollectorAction(tubcollector) {
                duty_cycle_ = tubcollector.getRobot().getSettingsParser().getDouble(name) ;
            }

            TubCollectorDutyCycleAction::~TubCollectorDutyCycleAction() {

            }
            void TubCollectorDutyCycleAction::start() {
                getTubCollector().setIntakePower(duty_cycle_) ;
            }

            void TubCollectorDutyCycleAction::run() {
            }

            bool TubCollectorDutyCycleAction::isDone() {
                return true ;
            }

            void TubCollectorDutyCycleAction::cancel() {
            }

            std::string TubCollectorDutyCycleAction::toString() {
                std::string result = "TubCollectorDutyCycleAction " ;
                result += std::to_string(duty_cycle_) ;
                return result ;
            }
    }
}
