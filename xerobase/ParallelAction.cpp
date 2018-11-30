#include "ParallelAction.h"

namespace xero {
    namespace base {
        /// \brief Start the directive; called once per directive when it starts
        void ParallelAction::start() {
            for(auto act : action_list_)
                act->start() ;
        }

        /// \brief Manage the directive; called each time through the robot loop
        void ParallelAction::run() {
            for(auto act : action_list_)
                act->run() ;
        }

        /// \brief Cancel the directive
        void ParallelAction::cancel() {
            for(auto act : action_list_) {
                if (!act->isDone())
                    act->cancel() ;
            }
        }

        /// \brief Return true if the directive is complete
        /// \returns True if the directive is complete
        bool ParallelAction::isDone() {
            bool done = true ;
            for(auto act : action_list_) {
                if (!act->isDone()) {
                    done = false ;
                    break ;
                }
            }

            return done  ;
        }

        /// \brief return a human readable string representing the action
        std::string ParallelAction::toString() {
            std::string ret("Parallel [") ;
            bool first = true ;

            for(auto act : action_list_) {
                if (!first)
                    ret += "," ;

                ret += act->toString() ;
                first = false ;
            }

            return ret ;
        }
    }
}