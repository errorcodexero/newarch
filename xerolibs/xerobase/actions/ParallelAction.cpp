#include "actions/ParallelAction.h"
#include <iostream>

namespace xero {
    namespace base {
        ParallelAction::ParallelAction(xero::misc::MessageLogger &logger) : CompositeAction(logger) {
        }

        ParallelAction::~ParallelAction() {            
        }

        /// \brief Start the action; called once per action when it starts
        /// This method calls the start method on each of the parallel actions to be executed
        void ParallelAction::start() {
            CompositeAction::start();
            for(auto act : action_list_)
                act->start() ;
        }

        /// \brief Manage the action; called each time through the robot loop
        /// This method calls the run method on each of the parallel actions to be executes
        void ParallelAction::run() {
            CompositeAction::run();

            bool done = true;
            for(auto act : action_list_)
            {
                if (!act->isDone())
                {
                    act->run() ;
                    if (!act->isDone())
                        done = false;
                }
            }

            if (done)
                setDone();
        }

        /// \brief Cancel the action
        /// This method calls the cancel method on each of the parallel actions to be executes
        void ParallelAction::cancel() {
            CompositeAction::cancel();
            for(auto act : action_list_) {
                if (!act->isDone()) {
                    act->cancel() ;
                }
            }
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable strinct
        std::string ParallelAction::toString() {
            std::string ret("Parallel [[") ;
            bool first = true ;

            for(auto act : action_list_) {
                if (!first)
                    ret += "," ;

                ret += std::to_string(act->getID()) ;
                ret += "{{" ;
                ret += act->toString() ;
                ret += "}}" ;
                first = false ;
            }

            ret += "]]" ;
            return ret ;
        }
    }
}