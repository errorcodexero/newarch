#include "DispatchAction.h"
#include "basegroups.h"
#include "Subsystem.h"
#include "Robot.h"
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
namespace base {

DispatchAction::DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block) : subsystem_(subsystem), action_(action), block_(block) {
	denied_ = false;
}

void DispatchAction::start() {
	if (!subsystem_->setAction(action_)) {
		MessageLogger &logger = subsystem_->getRobot().getMessageLogger() ;
		logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS) ;
		logger << "subsystem '" << subsystem_->getName() << "' rejected action '" ;
		logger << action_->toString() << "'" ;
		logger.endMessage() ;
		denied_ = true;
	}
}

void DispatchAction::run() {
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
	result += ", " ;
	result += block_ ? "BLOCK" : "NONBLOCKING" ;
	result += ")" ;
	return result ;
}

}
}
