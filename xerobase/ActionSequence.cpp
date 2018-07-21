#include "ActionSequence.h"

using namespace xero::base;


ActionSequence::ActionSequence(bool block) {
	block_ = block;
}

void ActionSequence::pushAction(std::shared_ptr<Action> action) {
	actionSequence_.push_back(action);
}

void ActionSequence::start() {
	index_ = actionSequence_.begin();
}

void ActionSequence::run() {
	while ((index_ != actionSequence_.end()) && ((*index_)->isDone() || (!(*index_)->block_))) {
		index_++;
		if (index_!=actionSequence_.end())
		(*index_)->run();
	}
}

bool ActionSequence::isDone() {
	return (index_ == actionSequence_.end());
}