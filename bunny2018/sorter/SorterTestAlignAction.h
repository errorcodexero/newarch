#pragma once

#include "SorterAlignCapableAction.h"
#include "Sorter.h"
#include <string>

namespace xero {
    namespace bunny2018 {
        class SorterTestAlignAction : public SorterAlignCapableAction {
        public:
            SorterTestAlignAction(Sorter &sorter, double delay, int desired) ;

            virtual ~SorterTestAlignAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;

		private:
			enum State {
				Align,
				Delay1,
				StartPertubate,
				Pertubate,
				Delay2
			} ;
            
        private:
			State state_ ;
			double start_ ;
			double duration_ ;
			double power_ ;
			bool is_done_ ;
			int total_ ;
			int desired_ ;
			bool dir_ ;
			double pertdur_ ;
			double start_align_ ;
        };
    }
}