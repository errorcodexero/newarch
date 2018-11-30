#include "SorterCalibrateAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterCalibrateAction::SorterCalibrateAction(Sorter &sorter) : SorterAction(sorter) {
				calibrate_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:calibration_power") ;
            }

            SorterCalibrateAction::~SorterCalibrateAction() {
            }

            void SorterCalibrateAction::start() {
				is_done_ = false ;
                getSubsystem().setCalibrated(false) ;
				getSubsystem().setSorterMotor(calibrate_power_) ;
            }

            void SorterCalibrateAction::run() {
				if (getSubsystem().getIndexState()) {
					getSubsystem().setCalibrated(true) ;
					is_done_ = true ;
					getSubsystem().setSorterMotor(0) ;
				}
            }

            bool SorterCalibrateAction::isDone() {
                return is_done_ ;
            }

            void SorterCalibrateAction::cancel() {
            }

            std::string SorterCalibrateAction::toString() {
                std::string result = "SorterCalibrateAction " ;
                return result ;
            }
    }
}
