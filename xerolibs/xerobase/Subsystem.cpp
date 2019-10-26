#include "Subsystem.h"
#include "actions/Action.h"
#include "actions/DispatchAction.h"
#include "Robot.h"
#include "basegroups.h"
#include <MessageLogger.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {

        Subsystem::Subsystem(Subsystem *parent, const std::string &name): Subsystem(parent->getRobot(), name) {
            parent_ = parent;
        }

        Subsystem::Subsystem(Robot &robot, const std::string &name) : 
        robot_(robot), name_(name) {
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
            
            if (action_ != nullptr && !action_->isDone()) {
                action_->run();
            } else if (!isRunningDefaultAction_) {
                // set the default action
                setAction(nullptr);
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

        bool Subsystem::_canAcceptAction(ActionPtr action) {
            if (auto composite = dynamic_cast<CompositeAction*>(action.get())) {
                for (auto child : composite->getChildren()) {
                    if (!_canAcceptAction(child)) return false;
                }
                return true;
            } else if (auto dispatch = dynamic_cast<DispatchAction*>(action.get())) {
                SubsystemPtr sub = dispatch->getSubsystem();
                
                // ensure the subsystem is one of our children
                Subsystem *current = sub.get();
                while (current != this) {
                    current = current->getParent();
                    if (current == nullptr) return false;
                }

                return sub->_canAcceptAction(dispatch->getAction());
            } else if (dynamic_cast<GenericAction*>(action.get())) {
                return true;
            } else {
                return canAcceptAction(action);
            }
        }

        Subsystem::SetActionResult Subsystem::setAction(ActionPtr action, bool isParent) {
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

            // No use replacing a null action with another null
            if (action_ == nullptr && action == nullptr) return SetActionResult::Accepted;

            if (action != nullptr && !_canAcceptAction(action)) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << getName() << "' rejected action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return SetActionResult::Rejected ;
            }

            // Don't accept the action if a parent is busy
            if (!isParent && parentBusy()) {
                MessageLogger &logger = getRobot().getMessageLogger();
                logger.startMessage(MessageLogger::MessageType::warning, MSG_GROUP_ACTIONS);
                logger << "Actions; subsystem '" << getName() 
                    << "' rejected action '" << action->toString() 
                    << "' because a parent subsystem is busy";
                logger.endMessage();
                return SetActionResult::ParentBusy;
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
            if (isRunningDefaultAction_) logger << " (default)";
            logger.endMessage() ;

            SetActionResult result = SetActionResult::Accepted;

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
                result = SetActionResult::PreviousCanceled;

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
            return result;
        }

        bool Subsystem::isBusy() {
            return action_ && !action_->isDone();
        }

        bool Subsystem::parentBusy() {
            auto parent = getParent();
            return parent && parent->isBusyOrParentBusy()
        }

        bool Subsystem::isBusyOrParentBusy() {
            return isBusy() || parentBusy();
        }

        bool Subsystem::isBusyOrChildBusy() {
            if (isBusy()) return true;
            for (auto child : children_) {
                if (child->isBusy()) return true;
            }
            return false;
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
