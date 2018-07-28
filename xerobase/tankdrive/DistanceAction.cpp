#include "DistanceAction.h"

using namespace xero::base::tankdrive;

DistanceAction::DistanceAction(TankDrive &parent, double target_distance) : TankDriveAction(parent) {
	target_distance_ = target_distance;
}

void DistanceAction::start() {
}

void DistanceAction::run() {
}

void DistanceAction::cancel() {
}

bool DistanceAction::isDone() {
	return true;
}

std::string DistanceAction::toString() {
	return "DistanceAction " + std::to_string(target_distance_);
}
