#include "Action.h"
#include "ITerminator.h"
#include "MessageLogger.h"

namespace xero{
    namespace base {
        class TerminateAction : public Action {
        public:
            TerminateAction(ActionPtr a, ITerminator &t, xero::misc::MessageLogger &logger) ;

            virtual void start() ;

            virtual void run() ;

            virtual void cancel() ;

            virtual bool isDone() ;

            virtual std::string toString() ;

        private: 
            ActionPtr action_ ;
            ITerminator& term_ ;
            bool is_done_ ;
            xero::misc::MessageLogger &logger_ ;
        };
    }
}