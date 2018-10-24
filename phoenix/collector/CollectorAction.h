#pragma once

#include "Action.h" 

namespace xero {
    namespace phoenix {
        class Collector;

		/// \brief the base class for all collector actions
        class CollectorAction : public xero::base::Action {
        public:
			/// \brief create a new collector action
			/// \partam collector the collector this action is for
            CollectorAction(Collector &collector);

			/// \brief destroy a collector action
            virtual ~CollectorAction(); 

        protected:
			/// \brief returns a reference to the collector associated with this action
			/// \return a reference to the associated collector
            Collector & getCollector() {
                return Collector_ ;
            }

        private:
			// The collector associated with this action
            Collector &Collector_ ;
        };
    }
}