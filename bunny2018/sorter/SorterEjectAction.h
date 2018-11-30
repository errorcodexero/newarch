#pragma once

#include "SorterAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterEjectAction : public SorterAction {
        public:
            SorterEjectAction(Sorter &sorter) ;

            virtual ~SorterEjectAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
			double eject_power_ ;
            double eject_duration_ ;
            double eject_start_ ;
			bool is_done_ ;
        };
    }
}