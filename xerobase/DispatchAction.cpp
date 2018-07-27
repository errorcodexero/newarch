#include "DispatchAction.h"

using namespace xero::base;

DispatchAction::DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block) : subsystem_(subsystem), action_(action), block_(block) {
	denied_ = false;
}

void DispatchAction::start() {
	if (!subsystem_->setAction(action_)) 
		denied_ = true;
}

void DispatchAction::run() {
	subsystem_->run();
}

bool DispatchAction::isDone() {
	return denied_ || !block_ || subsystem_->isDone();
}

void DispatchAction::cancel() {
	if (block_) {
		action_->cancel() ;
		subsystem_->setAction(nullptr) ;
	}
}

std::string DispatchAction::toString() {
	std::string result ;

	result = "DispatchAction(" ;
	result += subsystem_->getName() + "," + action_->toString() ;
	result += ")" ;
	return result ;
}