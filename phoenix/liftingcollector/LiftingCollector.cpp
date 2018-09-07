#include "LiftingCollector.h"
#include "Robot.h"


namespace xero {
	namespace phoenix {
		LiftingCollector::LiftingCollector(xero::base::Robot& robot) : xero::base::Subsystem(robot, "lifting collector") {
			lifter_ = std::make_shared<Lifter>(robot) ;

            collector_ = std::make_shared<Collector>(robot) ;

            addChild(lifter_);
            addChild(collector_);
		}

		LiftingCollector::~LiftingCollector(){
		}

		bool LiftingCollector::canAcceptAction(xero::base::ActionPtr action) {
			std::shared_ptr<LiftingCollectorAction> act_p = std::dynamic_pointer_cast<LiftingCollectorAction>(action) ;
			if (act_p == nullptr)
				return false ;

			// TODO: reject LiftingCollectorGoToHeightAction if not calibreated

			return true ;
		}
	}
}