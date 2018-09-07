#pragma once

#include <Subsystem.h>
#include <memory>
#include "collector/Collector.h"
#include "lifter/Lifter.h"

namespace xero {
	namespace phoenix {
		/// \brief this is the LiftingCollector subsystem
		class LiftingCollector : public xero::base::Subsystem {
		public:
			LiftingCollector(xero::base::Robot& robot);
			virtual ~LiftingCollector();

			std::shared_ptr<Lifter> getLifter(){
                return lifter_;
            }
			std::shared_ptr<Collector> getCollector(){
                return collector_;
            }
		private:
			std::shared_ptr<Lifter> lifter_;
			std::shared_ptr<Collector> collector_;
		};
	}
}