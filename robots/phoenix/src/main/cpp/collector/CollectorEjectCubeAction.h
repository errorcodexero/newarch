#pragma once

#include "CollectorAction.h"

namespace xero {
    namespace phoenix {
        class CollectorEjectCubeAction : public CollectorAction {
        public:
            CollectorEjectCubeAction(Collector &col, double speed) ;
            CollectorEjectCubeAction(Collector &col, const std::string &speed) ;
            CollectorEjectCubeAction(Collector &col, double speed, double delay) ;
            CollectorEjectCubeAction(Collector &col, const std::string &speed, const std::string &delay) ;

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() ;            

        private:
            bool sensor_ ;
            double speed_ ;
            double delay_ ;
            double start_ ;
        } ;
    }
}