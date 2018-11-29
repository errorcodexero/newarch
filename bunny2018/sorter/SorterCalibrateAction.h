#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterCalibrateAction : public SorterAction {
        public:
            SorterCalibrateAction(Sorter &sorter) ;

            virtual ~SorterCalibrateAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
        };
    }
}