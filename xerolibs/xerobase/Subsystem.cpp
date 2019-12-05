#include "Subsystem.h"
#include "actions/Action.h"
#include "actions/DispatchAction.h"
#include "Robot.h"
#include "basegroups.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {

        Subsystem::Subsystem(Subsystem *parent, const std::string &name): Subsystem(parent->getRobot(), name) {
            parent_ = parent;
        }

        Subsystem::Subsystem(Robot &robot, const std::string &name) : 
        robot_(robot), name_(name) {
            action_ = nullptr;
            parent_ = nullptr;
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

            frc::SmartDashboard::PutBoolean(name_, isBusy()) ;
        }

        void Subsystem::cancelAction() {
            if (action_ != nullptr)
                action_->cancel() ;
        }

        bool Subsystem::_canAcceptAction(ActionPtr action, bool isDefault) {
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
                return isDefault ? canAcceptDefaultAction(action) : canAcceptAction(action);
            }
        }
        
        bool Subsystem::setDefaultAction(ActionPtr action) {
            if (_canAcceptAction(action, /*isDefault=*/true)) {
                defaultAction_ = action;
                if (!isBusy()) setAction(nullptr, /*allowParentBusy=*/true);
                return true;
            } else {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << getName() << "' rejected default action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return false ;
            }
        }

        Subsystem::SetActionResult Subsystem::setAction(ActionPtr action, bool allowParentBusy) {
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

            // Don't replace a null action with another null
            if (action_ == nullptr && action == nullptr) return SetActionResult::Accepted;

            if (action != nullptr && !_canAcceptAction(action)) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << getName() << "' rejected action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return SetActionResult::Rejected ;
            }

            // Don't accept the action if a parent is busy
            if (!allowParentBusy && parentBusy() && action != nullptr && !isRunningDefaultAction_) {
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

            // Cancel any currently-running actions
            SetActionResult result;
            if (cancelActionsAndChildActions(action)) result = SetActionResult::PreviousCanceled;
            else result = SetActionResult::Accepted;

            // And now start the Action
            //
            action_ = action ;
            if (action_ != nullptr) {
                action_->start() ;
            }
            return result;
        }


        bool Subsystem::cancelActionsAndChildActions(std::shared_ptr<Action> action) {
            bool canceled = false;
            if (isBusy()) {
                auto &logger = getRobot().getMessageLogger();
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE);
                logger << "Actions: subsystem '" << getName() << "' overriding action '" << action_->toString();
                logger << "' with action '" << (action == nullptr ? "null" : action->toString()) << "'" ;
                logger.endMessage();
                cancelAction();
                canceled = true;

                if (isBusy()) {
                    // The action didn't cancel (even though it should have)
                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::warning, MSG_GROUP_ACTIONS) ;
                    logger << "Actions: subsystem '" << getName() << "' action '" << action_->toString();
                    logger << "' did not cancel immediately; proceeding anyway." ;
                    logger.endMessage() ;
                }
            }
            for (auto child : children_) canceled = child->cancelActionsAndChildActions(nullptr) || canceled;
            return canceled;
        }

        bool Subsystem::isBusy() {
            return !isRunningDefaultAction_ && action_ && !action_->isDone();
        }

        bool Subsystem::parentBusy() {
            auto parent = getParent();
            return parent && parent->isBusyOrParentBusy();
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
