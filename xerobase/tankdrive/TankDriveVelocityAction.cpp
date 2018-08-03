#include "TankDriveVelocityAction.h"

using namespace xero::base;

TankDriveVelocityAction::TankDriveVelocityAction(TankDrive &tank_drive, double target_velocity) : TankDriveAction(tank_drive) {
	target_velocity_ = target_velocity;
}

void TankDriveVelocityAction::start() {
}

void TankDriveVelocityAction::run() {
}

void TankDriveVelocityAction::cancel() {
}

bool TankDriveVelocityAction::isDone() {
	return true;
}

std::string TankDriveVelocityAction::toString() {
	return "TankDriveVelocityAction " + std::to_string(target_velocity_);
}
