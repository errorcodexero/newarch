#include "ActionSequence.h"

using namespace xero::base;


ActionSequence::ActionSequence() {
	isDone_ = false;
}

void ActionSequence::pushAction(ActionPtr action) {
	actionSequence_.push_back(action);
}

void ActionSequence::start() {
	index_ = actionSequence_.begin();
}

bool ActionSequence::performActionForIsDoneStatus(ActionPtr action) {
	action->start();
	action->run();
	return action->isDone();
}

void ActionSequence::run() {
	while ((index_ != actionSequence_.end()) && performActionForIsDoneStatus(*index_)) {
		index_++;
	}
}

bool ActionSequence::isDone() {
	return isDone || (index_ == actionSequence_.end());
}

bool ActionSequence::cancel() {
	isDone_ = true;
	return (*index_)->cancel();
}