#include "Subsystem.h"
#include "actions/Action.h"
#include "Robot.h"
#include "basegroups.h"
#include <MessageLogger.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {

        Subsystem::Subsystem(Robot &robot, const std::string &name) : robot_(robot) , name_(name) {
            action_ = nullptr ;
        }

        Subsystem::~Subsystem() {
        }

        int Subsystem::initPlot(const std::string &name) {
            return getRobot().getPlotManager().initPlot(name) ;
        }

        void Subsystem::startPlot(int id, const std::vector<std::string> &cols) {
            getRobot().getPlotManager().startPlot(id, cols) ;
        }

        void Subsystem::addPlotData(int id, const std::vector<double> &values) {
            getRobot().getPlotManager().addPlotData(id, values) ;
        }

        void Subsystem::endPlot(int id) {
            getRobot().getPlotManager().endPlot(id) ;
        }

        void Subsystem::run() {
            for(auto sub: children_) {
                sub->run() ;
            }
            
            if (action_ != nullptr){
                action_->run() ;
                if (pending_ != nullptr && action_->isDone()) {
                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                    logger << "Actions: subsystem '" << getName() << "' pending action '" << pending_->toString() ;
                    logger << "' was started" ;
                    logger.endMessage() ;

                    action_ = pending_ ;
                    pending_ = nullptr ;
                    action_->start() ;
                }
            }
        }
        
        void Subsystem::computeState() {
            for(auto sub: children_)
                sub->computeState() ;     
        }

        void Subsystem::cancelAction() {
            if (action_ != nullptr)
                action_->cancel() ;
        }

        SetActionResult Subsystem::setAction(ActionPtr action) {
            //
            // Check that the action is valid for this subsystem.  If not, print and error
            // and do nothing else.  Any existing action remains attached to the subsystem and
            // continue to perform its function.
            //
            if (action == nullptr) {
                action = getDefaultAction();
                isRunningDefaultAction_ = true;
            } else {
                isRunningDefaultAction_ = false;
            }

            if (action != nullptr && !canAcceptAction(action)) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << getName() << "' rejected action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return SetActionResult::Rejected ;
            }

            //
            // Print information about what is being requested
            //
            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
            if (action == nullptr)
                logger << "Actions: subsystem '" << getName() << "' was assigned NULL action" ;
            else
                logger << "Actions: subsystem '" << getName() << "' was assigned action '" << action->toString() << "'" ;    
            if (isRunningDefaultAction_) logger << " (default action)";
            logger.endMessage() ;            

            if (action_ != nullptr && !action_->isDone()) {
                //
                // The current Action is still running, interrupt it
                //
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
                logger << "Actions: subsystem '" << getName() << "' action '" << action_->toString() << "' was canceled" ;
                logger.endMessage() ; 

                //
                // Cancel the action and start the new action.
                //
                cancelAction();

                if(!action_->isDone()) {
                    //
                    // The old action did not finish immedately after cancel was called; log an error but proceed anyway.
                    //

                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS) ;
                    logger << "Actions: subsystem '" << getName() << "' action '" << action_->toString();
                    logger << "' did not cancel immediately; proceeding anyway." ;
                    logger.endMessage() ;                    
                }
            }
            // And now start the Action
            //
            action_ = action ;
            if (action_ != nullptr) {
                action_->start() ;
            }
            return true ;
        }

        void Subsystem::reset() {
            action_ = nullptr ;

            for(auto child: children_)
                child->reset() ;
        }

        void Subsystem::init(LoopType ltype) {
            for(auto child: children_) {
                child->init(ltype) ;
            }
        }
    }
}
