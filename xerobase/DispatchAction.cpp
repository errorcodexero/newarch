#include "DispatchAction.h"

using namespace xero::base;

DispatchAction::DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block) : subsystem_(subsystem), action_(action), block_(block) {
}

void DispatchAction::start() {
	subsystem_->setAction(action_);
}

void DispatchAction::run() {
	subsystem_->run();
}

bool DispatchAction::isDone() {
	return denied_ || !block_ || subsystem_->isDone();
}

void DispatchAction::end() {

}

bool DispatchAction::cancel() {

}