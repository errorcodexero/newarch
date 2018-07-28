#include "VelocityAction.h"

using namespace xero::base::tankdrive;

VelocityAction::VelocityAction(TankDrive &parent, double target_velocity) : TankDriveAction(parent) {
	target_velocity_ = target_velocity;
}

void VelocityAction::start() {
}

void VelocityAction::run() {
}

void VelocityAction::cancel() {
}

bool VelocityAction::isDone() {
	return true;
}

std::string VelocityAction::toString() {
	return "VelocityAction " + std::to_string(target_velocity_);
}
