#include "TubCollectorEjectTubAction.h"
#include "TubCollector.h"
//#include "Intake.h"
#include "TubCollectorDutyCycleAction.h"
#include <Robot.h>

namespace xero {
    namespace ranseur {

        TubCollectorEjectTubAction::TubCollectorEjectTubAction(TubCollector &tubcol, double speed, double delay) : TubCollectorAction(tubcol) {
            speed_ = speed ;
            delay_ = delay ;
        }

        TubCollectorEjectTubAction::TubCollectorEjectTubAction(TubCollector &tubcol, const std::string &speed, const std::string &delay) : TubCollectorAction(tubcol) {
            speed_ = getTubCollector().getRobot().getSettingsParser().getDouble(speed) ;
            delay_ = getTubCollector().getRobot().getSettingsParser().getDouble(delay) ;
        }

        void TubCollectorEjectTubAction::start() {
            getTubCollector().getIntake()->set(speed_) ;
            start_ = getTubCollector().getRobot().getTime() ;
            isdone_ = false ;
        }

        void TubCollectorEjectTubAction::run() {
           if (getTubCollector().getRobot().getTime() - start_ > delay_) {
               getTubCollector().getIntake()->set(0.0) ;
               isdone_ = true ;
               getTubCollector().closeHand() ;
           }
        }

        bool TubCollectorEjectTubAction::isDone() {
            return isdone_ ;
        }

        void TubCollectorEjectTubAction::cancel() {
           getTubCollector().getIntake()->set(0.0) ;
           isdone_ = true ;    
        }

        std::string TubCollectorEjectTubAction::toString() {
            std::string result = "TubCollectorDutyCycleAction " ;
            result += std::to_string (speed_) ;
            result += " " ;
            result += std::to_string (delay_) ;
            return result ;
        }
    }
}
