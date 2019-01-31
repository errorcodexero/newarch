#include "Action.h"
#include "ITerminator.h"

namespace xero{
    namespace base {
        class TerminateAction:public Action{
        public:
            TerminateAction(ActionPtr a, ITerminator &t) ;

            virtual void start() ;

            virtual void run() ;

            virtual void cancel() ;

            virtual bool isDone() ;

            virtual std::string toString() ;

        private: 
            ActionPtr action_ ;
            ITerminator& term_ ;
            bool is_done_ ;
        };
    }
}