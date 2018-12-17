#include "SorterPowerAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterPowerAction::SorterPowerAction(Sorter &sorter, double v, bool smotor) : SorterAction(sorter) {
                power_ = v ;
                sorter_ = smotor ;
            }

            SorterPowerAction::SorterPowerAction(Sorter &sorter, const std::string &name, bool smotor) : SorterAction(sorter) {
                power_ = sorter.getRobot().getSettingsParser().getDouble(name) ;
                sorter_ = smotor;
            }

            SorterPowerAction::~SorterPowerAction() {

            }
            void SorterPowerAction::start() {
                if (sorter_) {
                	getSubsystem().setSorterMotor(power_) ;
                } else {
                    getSubsystem().setIntakeMotor(power_) ;
                }
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
