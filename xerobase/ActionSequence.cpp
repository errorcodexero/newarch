#include "ActionSequence.h"
#include "basegroups.h"

using namespace xero::misc ;

namespace xero
{
namespace base
{

ActionSequence::ActionSequence(xero::misc::MessageLogger &logger, const std::string &name) : logger_(logger) , name_(name)
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
	if (index_ < actionSequence_.size())
	{
		logger_.startMessage(MessageLogger::MessageType::debug, group_) ;
		logger_ << "starting action " << index_ << " of " << actionSequence_.size() - 1 ;
		logger_ << " '" << actionSequence_[index_]->toString() << "'" ;
		logger_.endMessage() ;
		actionSequence_[index_]->start();
	}
	else {
		//
		// We have reached the end of the sequence
		//
		isDone_ = true ;
	}
};

void ActionSequence::run()
{
	while (1)
	{
		if (index_ == -1 || actionSequence_[index_]->isDone())
		{
			startNextAction();
			if (index_ == actionSequence_.size())
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
	result += name_;
	result += " [" ;
	for(size_t i = 0 ; i < actionSequence_.size() ; i++) {
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
