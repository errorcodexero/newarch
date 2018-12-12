#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterPowerAction : public SorterAction {
        public:
            SorterPowerAction(Sorter &sorter, double power) ;

            SorterPowerAction(Sorter &sorter, const std::string &name) ;

            virtual ~SorterPowerAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
            double power_ ;
        };
    }
}