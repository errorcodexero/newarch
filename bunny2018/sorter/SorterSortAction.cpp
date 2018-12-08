#include "SorterSortAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterSortAction::SorterSortAction(Sorter &sorter, Sorter::BallColor color) : SorterAlignCapableAction(sorter) {
				color_ = color ;
            }

            SorterSortAction::~SorterSortAction() {
            }

            void SorterSortAction::start() {
            }

            void SorterSortAction::run() {
            }

            bool SorterSortAction::isDone() {
                return true ;
            }

            void SorterSortAction::cancel() {
            }

            std::string SorterSortAction::toString() {
                std::string result = "SorterSortAction " ;
				if (color_ == Sorter::BallColor::Red)
					result += "red" ;
				else if (color_ == Sorter::BallColor::Blue)
					result += "blue" ;
				else
					result = "all" ;

                return result ;
            }
    }
}
