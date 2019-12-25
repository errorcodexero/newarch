#pragma once

#include <Subsystem.h>

namespace xero {
    namespace phoenix {
        class Lifter ;
        class Collector ;

        class LiftingCollector: public xero::base::Subsystem {
        public:
            class LiftingCollectorAction : public xero::base::Action {                
            } ;

        public:
            LiftingCollector(xero::base::Subsystem *parent) ;
            virtual ~LiftingCollector() ;

            virtual void createNamedSequences() ;
            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

            std::shared_ptr<Lifter> getLifter() {
                return lifter_ ;
            }

            std::shared_ptr<Collector> getCollector() {
                return collector_ ;
            }

        private:
            std::shared_ptr<Lifter> lifter_ ;
            std::shared_ptr<Collector> collector_ ;
        } ;
    }
}