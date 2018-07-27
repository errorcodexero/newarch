#include "ActionSequence.h"

namespace xero
{
namespace base
{

ActionSequence::ActionSequence()
{
	isDone_ = false;
}

void ActionSequence::pushAction(ActionPtr action)
{
	actionSequence_.push_back(action);
}

void ActionSequence::start()
{
	index_ = -1;
}

void ActionSequence::startNextAction()
{
	index_++;
	if (index_ < actionSequence_.size())
	{
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

bool ActionSequence::cancel()
{
	isDone_ = true;
	return (actionSequence_[index_])->cancel();
}

std::string ActionSequence::toString()
{
	std::string result ;

	result += "[" ;
	for(size_t i = 0 ; i < actionSequence_.size() ; i++) {
		if (i != 0)
			result += "," ;
		result += actionSequence_[i]->toString() ;
	}
	
	return result ;
}

void ActionSequence::end()
{
}

void ActionSequence::pushSubActionPair(SubsystemPtr subsystem, ActionPtr action, bool block)
{
	auto p = std::make_shared<DispatchAction>(subsystem, action, block);
	pushAction(p);
}

} // namespace base
} // namespace xero
