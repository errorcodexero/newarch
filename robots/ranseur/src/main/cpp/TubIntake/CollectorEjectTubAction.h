#pragma once

#include "CollectorAction.h"

namespace xero {
    namespace ranseur {
        class CollectorEjectTubAction : public CollectorAction {
        public:
            CollectorEjectTubAction(Collector &col, double speed) ;
            CollectorEjectTubAction(Collector &col, const std::string &speed) ;
            CollectorEjectTubAction(Collector &col, double speed, double delay) ;
            CollectorEjectTubAction(Collector &col, const std::string &speed, const std::string &delay) ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;            

        private:
            bool sensor_ ;
            double speed_ ;
            double delay_ ;
            bool isdone_ ;
            double start_ ;
        } ;
    }
}