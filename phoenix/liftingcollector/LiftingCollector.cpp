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
	}
}