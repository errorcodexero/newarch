#include "SorterCalibrateAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterCalibrateAction::SorterCalibrateAction(Sorter &sorter) : SorterAction(sorter) {
            }

            SorterCalibrateAction::~SorterCalibrateAction() {
            }

            void SorterCalibrateAction::start() {
                getSubsystem().encoder_->Reset();
            }

            void SorterCalibrateAction::run() {
            }

            bool SorterCalibrateAction::isDone() {
                return true;
            }

            void SorterCalibrateAction::cancel() {
            }

            std::string SorterCalibrateAction::toString() {
                std::string result = "SorterCalibrateAction " ;
                return result ;
            }
    }
}
