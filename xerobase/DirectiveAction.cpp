#include "DirectiveAction.h"

using namespace xero::base;

DirectiveAction::DirectiveAction(std::shared_ptr<Subsystem> subsystem, std::shared_ptr<Action> directive, bool block) : subsystem_(subsystem), directive_(directive) {
	block_ = block;
}

void DirectiveAction::start() {
	subsystem_->setDirective(directive_);
}

void DirectiveAction::run() {
	subsystem_->run();
}

bool DirectiveAction::isDone() {
	return subsystem_->isDone();
}

void DirectiveAction::end() {

}

bool DirectiveAction::cancel() {

}