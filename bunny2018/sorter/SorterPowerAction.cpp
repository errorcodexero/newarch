#include "SorterPowerAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterPowerAction::SorterPowerAction(Sorter &sorter, double v) : SorterAction(sorter) {
                power_ = v ;
            }

            SorterPowerAction::SorterPowerAction(Sorter &sorter, const std::string &name) : SorterAction(sorter) {
                power_ = sorter.getRobot().getSettingsParser().getDouble(name) ;
            }

            SorterPowerAction::~SorterPowerAction() {

            }
            void SorterPowerAction::start() {
            	getSubsystem().setSorterMotor(power_) ;
            }

            void SorterPowerAction::run() {
            }

            bool SorterPowerAction::isDone() {
                return true ;
            }

            void SorterPowerAction::cancel() {
            }

            std::string SorterPowerAction::toString() {
                std::string result = "SorterPowerAction " ;
                result += std::to_string(power_) ;
                return result ;
            }
    }
}
