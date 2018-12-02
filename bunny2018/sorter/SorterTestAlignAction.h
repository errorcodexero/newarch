#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterTestAlignAction : public SorterAction {
        public:
            SorterTestAlignAction(Sorter &sorter) ;

            virtual ~SorterTestAlignAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
        };
    }
}