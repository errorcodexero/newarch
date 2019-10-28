#include "actions/ParallelAction.h"
#include <iostream>

namespace xero {
    namespace base {
        ParallelAction::ParallelAction() {
        }

        ParallelAction::~ParallelAction() {            
        }

        /// \brief Start the action; called once per action when it starts
        /// This method calls the start method on each of the parallel actions to be executed
        void ParallelAction::start() {
            for(auto act : action_list_)
                act->start() ;
        }

        /// \brief Manage the action; called each time through the robot loop
        /// This method calls the run method on each of the parallel actions to be executes
        void ParallelAction::run() {
            for(auto act : action_list_)
                act->run() ;
        }

        /// \brief Cancel the action
        /// This method calls the cancel method on each of the parallel actions to be executes
        void ParallelAction::cancel() {
            for(auto act : action_list_) {
                if (!act->isDone()) {
                    act->cancel() ;
                }
            }
        }

        /// \brief Return true if the action is complete.
        /// This method returns true only when all of the parallel actions are complete.
        /// \returns True if the action is complete
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
        /// \returns a human readable strinct
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