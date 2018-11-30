#include "SorterEjectAction.h"
#include "Sorter.h"
#include <Robot.h>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterEjectAction::SorterEjectAction(Sorter &sorter) : SorterAction(sorter) {
				eject_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:eject_power") ;
				eject_duration_ = sorter.getRobot().getSettingsParser().getDouble("sorter:eject_duration") ;
            }

            SorterEjectAction::~SorterEjectAction() {
            }

            void SorterEjectAction::start() {
				is_done_ = false ;
				getSubsystem().setOuttakeMotor(eject_power_) ;
                eject_start_ = getSubsystem().getRobot().getTime() ;
            }

            void SorterEjectAction::run() {
                if (getSubsystem().getRobot().getTime() - eject_start_ > eject_duration_) {
                    is_done_ = true ;
                    getSubsystem().setOuttakeMotor(0.0) ;
                }
            }

            bool SorterEjectAction::isDone() {
                return is_done_ ;
            }

            void SorterEjectAction::cancel() {
            }

            std::string SorterEjectAction::toString() {
                std::string result = "SorterEjectAction " ;
                return result ;
            }
    }
}
