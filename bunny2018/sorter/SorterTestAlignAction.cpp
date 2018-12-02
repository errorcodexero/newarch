#include "SorterTestAlignAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterTestAlignAction::SorterTestAlignAction(Sorter &sorter) : SorterAction(sorter) {
            }

            SorterTestAlignAction::~SorterTestAlignAction() {
            }
            void SorterTestAlignAction::start() {
            }

            void SorterTestAlignAction::run() {
            }

            bool SorterTestAlignAction::isDone() {
                return true ;
            }

            void SorterTestAlignAction::cancel() {
            }

            std::string SorterTestAlignAction::toString() {
                std::string result = "SorterTestAlignAction " ;
                return result ;
            }
    }
}
