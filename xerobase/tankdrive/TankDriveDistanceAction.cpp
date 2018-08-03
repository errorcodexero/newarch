#include "TankDriveDistanceAction.h"

#include <math.h>

#include <SettingsParser.h>
#include "Robot.h"

using namespace xero::base;

TankDriveDistanceAction::TankDriveDistanceAction(TankDrive &tank_drive, double target_distance) : TankDriveAction(tank_drive) {
	target_distance_ = target_distance;	
	is_done_ = false;

	has_stalled_ = false;
}

void TankDriveDistanceAction::start() {
	initial_dist_ = tank_drive_.getDist();

	xero::misc::SettingsParser parser = tank_drive_.getRobot().getSettingsParser();

	distance_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:dist_pid");
	angle_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:angle_pid", true);

	stall_monitor_.initFromSettings(parser, "tankdrive:distance_action:stall_monitor");

	distance_threshold_ = parser.getDouble("tankdrive:distanceaction:distance_threshold");

	tank_drive_.navx_.ZeroYaw();
}

void TankDriveDistanceAction::run() {
	double current_distance = tank_drive_.getDist();
	double distance_travelled = current_distance - initial_dist_;
	double remaining_distance = target_distance_ - distance_travelled;

	if (std::fabs(remaining_distance) > distance_threshold_) {
		stall_monitor_.addSample(current_distance);
		if (stall_monitor_.isStalled()) {
			if (has_stalled_) {
				xero::misc::SettingsParser parser = tank_drive_.getRobot().getSettingsParser();
				distance_pid_.initFromSettingsExtended(parser, "tankdrive:distanceaction:dist_stall_pid");
				stall_monitor_.reset();
				has_stalled_ = true;
			}
		}

		double delta_time = tank_drive_.getRobot().getDeltaTime();

		double base_power = distance_pid_.getOutput(target_distance_, distance_travelled, delta_time);

		double current_angle = tank_drive_.getAngle();
		double straightness_offset = angle_pid_.getOutput(0, current_angle, delta_time);
		double left_power = base_power - straightness_offset;
		double right_power = base_power + straightness_offset;

		tank_drive_.setMotorsToPercents(left_power, right_power);
	} else {
		is_done_ = true;
	}
}

void TankDriveDistanceAction::cancel() {
}

bool TankDriveDistanceAction::isDone() {
	return is_done_;
}

std::string TankDriveDistanceAction::toString() {
	return "TankDriveDistanceAction " + std::to_string(target_distance_);
}
