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
            TubCollectorAction::start();

            getTubCollector().setIntakePower(speed_) ;
            start_ = getTubCollector().getRobot().getTime() ;
        }

        void TubCollectorEjectTubAction::run() {
            TubCollectorAction::run();

            if (getTubCollector().getRobot().getTime() - start_ > delay_)
            {
                getTubCollector().setIntakePower(0.0);
                getTubCollector().closeHand();
                setDone();
           }
        }

        void TubCollectorEjectTubAction::cancel() {
            TubCollectorAction::cancel();
            getTubCollector().setIntakePower(0.0) ;
            setDone();
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
