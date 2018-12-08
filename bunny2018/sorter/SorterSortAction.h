#pragma once

#include "SorterAlignCapableAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterSortAction : public SorterAlignCapableAction {
        public:
            SorterSortAction(Sorter &sorter, Sorter::BallColor color) ;

            virtual ~SorterSortAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;

		private:
			Sorter::BallColor color_ ;
        };
    }
}