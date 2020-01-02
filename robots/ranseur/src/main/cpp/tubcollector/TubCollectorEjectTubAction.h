#pragma once

#include "TubCollectorAction.h"

namespace xero {
    namespace ranseur {
        class TubCollectorEjectTubAction : public TubCollectorAction {
        public:
            TubCollectorEjectTubAction(TubCollector &tubcol, double speed, double delay) ;
            TubCollectorEjectTubAction(TubCollector &tubcol, const std::string &speed, const std::string &delay) ;

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() ;            

        private:
            double speed_ ;
            double delay_ ;
            double start_ ;
        } ;
    }
}
