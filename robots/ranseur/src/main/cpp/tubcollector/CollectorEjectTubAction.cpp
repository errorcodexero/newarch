#include "CollectorEjectTubAction.h"
#include "Collector.h"
//#include "Intake.h"
#include "CollectorDutyCycleAction.h"
#include <Robot.h>

namespace xero {
    namespace ranseur {

        CollectorEjectTubAction::CollectorEjectTubAction(Collector &col, double speed, double delay) : CollectorAction(col) {
            speed_ = speed ;
            delay_ = delay ;
        }

        CollectorEjectTubAction::CollectorEjectTubAction(Collector &col, const std::string &speed, const std::string &delay) : CollectorAction(col) {
            speed_ = getCollector().getRobot().getSettingsParser().getDouble(speed) ;
            delay_ = getCollector().getRobot().getSettingsParser().getDouble(delay) ;
        }

        void CollectorEjectTubAction::start() {
            getCollector().getIntake()->set(speed_) ;
            start_ = getCollector().getRobot().getTime() ;
            isdone_ = false ;
        }

        void CollectorEjectTubAction::run() {
           if (getCollector().getRobot().getTime() - start_ > delay_) {
               getCollector().getIntake()->set(0.0) ;
               isdone_ = true ;
               getCollector().closeHand() ;
           }
        }

        bool CollectorEjectTubAction::isDone() {
            return isdone_ ;
        }

        void CollectorEjectTubAction::cancel() {
           getCollector().getIntake()->set(0.0) ;
           isdone_ = true ;    
        }

        std::string CollectorEjectTubAction::toString() {
            std::string result = "CollectorDutyCycleAction " ;
            result += std::to_string (speed_) ;
            result += " " ;
            result += std::to_string (delay_) ;
            return result ;
        }
    }
}
