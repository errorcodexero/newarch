#pragma once

#include <Subsystem.h>
#include <memory>
#include <Solenoid.h>
#include "VictorSP.h"
#include "DigitalInput.h"
#include "Encoder.h"
#include "LiftingCollectorAction.h"
#include "collector/Collector.h"
#include "lifter/Lifter.h"

namespace xero {
	namespace phoenix {
		/// \brief this is the LiftingCollector subsystem
		class LiftingCollector : public xero::base::Subsystem {
		public:
			LiftingCollector(xero::base::Robot& robot);
			virtual ~LiftingCollector();
			virtual void computeState() ;

			std::shared_ptr<Lifter> getLifter(){
                return lifter_;
            }
			std::shared_ptr<Collector> getCollector(){
                return collector_;
            }
		protected:
			/// \brief Determine if the LiftingCollector subsystem can accept the given action.
            /// For this subsystem the only critera is that the action be derived from
            /// the LiftingCollectorAction type.
			virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

		private:
			std::shared_ptr<Lifter> lifter_;
			std::shared_ptr<Collector> collector_;
		};
	}
}