#include "Action.h"
#include "ITerminator.h"
#include "MessageLogger.h"
#include <list>
#include <memory>

namespace xero{
    namespace base {
        class TerminateAction : public Action {
        public:
            TerminateAction(ActionPtr a, xero::misc::MessageLogger &logger) ;

            virtual void start() ;

            virtual void run() ;

            virtual void cancel() ;

            virtual bool isDone() ;

            virtual std::string toString() ;

            void addTerminator(std::shared_ptr<ITerminator> term) {
                terminators_.push_back(term) ;
            }

        private: 
            ActionPtr action_ ;
            std::list<std::shared_ptr<ITerminator>> terminators_ ;
            bool is_done_ ;
            xero::misc::MessageLogger &logger_ ;
        };

        typedef std::shared_ptr<TerminateAction> TermActionPtr ;
    }
}