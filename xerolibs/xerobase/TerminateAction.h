#include "Action.h"
#include "ITerminator.h"
#include "MessageLogger.h"
#include <list>
#include "Robot.h"
#include <memory>

namespace xero{
    namespace base {
        class TerminateAction : public Action {
        public:
            TerminateAction(ActionPtr a, Robot &robot, double delay = 0.0) ;

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
            Robot &robot_ ;
            double delay_ ;
            double start_ ;
        };

        typedef std::shared_ptr<TerminateAction> TermActionPtr ;
    }
}