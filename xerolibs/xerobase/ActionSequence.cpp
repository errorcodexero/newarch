#include "ActionSequence.h"
#include "basegroups.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero
{
namespace base
{

ActionSequence::ActionSequence(xero::misc::MessageLogger &logger) : logger_(logger) 
{
    isDone_ = false;
    group_ = MSG_GROUP_ACTIONS ;
}

void ActionSequence::pushAction(ActionPtr action)
{
    actionSequence_.push_back(action);
}

void ActionSequence::start()
{
    index_ = -1 ;
}

void ActionSequence::startNextAction()
{
    index_++;
    if (static_cast<size_t>(index_) < actionSequence_.size())
    {
        assert(group_ != 0) ;
        logger_.startMessage(MessageLogger::MessageType::debug, group_) ;
        logger_ << "Actions: starting " << index_ << " of " << actionSequence_.size() - 1 ;
        logger_ << " '" << actionSequence_[index_]->toString() << "'" ;
        logger_.endMessage() ;
        actionSequence_[index_]->start();
        frc::SmartDashboard::PutString("Step", actionSequence_[index_]->toString()) ;
    }
    else {
        //
        // We have reached the end of the sequence
        //
        isDone_ = true ;
    }
}

void ActionSequence::run()
{
    while (1)
    {
        if (index_ == static_cast<int>(actionSequence_.size()))
            break ;

        if (index_ == -1 || actionSequence_[index_]->isDone())
        {
            startNextAction();
            if (static_cast<size_t>(index_) == actionSequence_.size())
                break;
        };
        actionSequence_[index_]->run();
        if (!actionSequence_[index_]->isDone())
        {
            break;
        }
    }
}

bool ActionSequence::isDone()
{
    return isDone_ ;
}

void ActionSequence::cancel()
{
    isDone_ = true;
    actionSequence_[index_]->cancel() ;
}

std::string ActionSequence::toString()
{
    std::string result ;
    result += " [" ;
    for(size_t i = 0 ; i < actionSequence_.size() ; i++) {
        auto ptr = actionSequence_[i] ;
        if (i != 0)
            result += "," ;
        result += actionSequence_[i]->toString() ;
    }
    result += "]" ;
    return result ;
}

void ActionSequence::pushSubActionPair(SubsystemPtr subsystem, ActionPtr action, bool block)
{
    auto p = std::make_shared<DispatchAction>(subsystem, action, block);
    pushAction(p);
}

} // namespace base
} // namespace xero
