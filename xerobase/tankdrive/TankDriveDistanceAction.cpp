#include "TankDriveDistanceAction.h"

#include <math.h>

#include <SettingsParser.h>
#include "Robot.h"
#include "basegroups.h"

using namespace xero::misc;

namespace xero {
namespace base {

TankDriveDistanceAction::TankDriveDistanceAction(TankDrive &tank_drive, double target_distance) : TankDriveAction(tank_drive) {
	target_distance_ = target_distance;	
	is_done_ = false;

	has_stalled_ = false;
	double maxa = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:straight:maxa") ;
	double maxd = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:straight:maxd") ;
	double maxv = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:straight:maxv") ;		
	profile_ = new TrapezoidalProfile(maxa, maxd, maxv) ;
}

TankDriveDistanceAction::~TankDriveDistanceAction() {
	if (profile_ != nullptr)
		delete profile_ ;
}

void TankDriveDistanceAction::start() {
	initial_dist_ = getTankDrive().getDist();

	xero::misc::SettingsParser parser = getTankDrive().getRobot().getSettingsParser();

	velocity_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:velocity_pid");
	angle_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:angle_pid", true);

	stall_monitor_.initFromSettings(parser, "tankdrive:distance_action:stall_monitor");

	distance_threshold_ = parser.getDouble("tankdrive:distanceaction:distance_threshold");
	profile_outdated_error_ = parser.getDouble("tankdrive:distanceaction:profile_outdated_error");

	start_time_ = getTankDrive().getRobot().getTime();

	getTankDrive().navx_->ZeroYaw();
}

void TankDriveDistanceAction::run() {
	MessageLogger &logger = getTankDrive().getRobot().getMessageLogger();

	if (!is_done_) {
		double current_distance = getTankDrive().getDist();
		double distance_travelled = current_distance - initial_dist_;
		double remaining_distance = target_distance_ - distance_travelled;

		if (std::fabs(remaining_distance) > distance_threshold_) {
			stall_monitor_.addSample(current_distance);
			if (stall_monitor_.isStalled()) {
				if (!has_stalled_) {
					xero::misc::SettingsParser parser = getTankDrive().getRobot().getSettingsParser();
					velocity_pid_.initFromSettingsExtended(parser, "tankdrive:distanceaction:velocity_stall_pid");
					stall_monitor_.reset();
					has_stalled_ = true;

					logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
					logger << "Drivebase stalled, switching to stall PID parameters: " << velocity_pid_.toString();
					logger.endMessage();
				} else {
					logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
					logger << "Drivebase stalled despite having already switched to stall PID parameters";
					logger.endMessage();
				}
			}

			double delta_time = getTankDrive().getRobot().getDeltaTime();
			double target_distance = profile_->getDistance(delta_time);
			double profile_error = std::fabs(target_distance - distance_travelled);

			double current_velocity = getTankDrive().getVelocity();

			if (profile_error > profile_outdated_error_) {
				profile_->update(remaining_distance, current_velocity, 0.0);

				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
				logger << "Fell behind velocity profile, updating profile";
				logger.endMessage();
			}

			double target_velocity = profile_->getSpeed(delta_time);

			double base_power = velocity_pid_.getOutput(target_velocity, current_velocity, delta_time);

			double current_angle = getTankDrive().getAngle();
			double straightness_offset = angle_pid_.getOutput(0, current_angle, delta_time);
			double left_power = base_power - straightness_offset;
			double right_power = base_power + straightness_offset;

			getTankDrive().setMotorsToPercents(left_power, right_power);
		} else {
			is_done_ = true;

			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
			logger << "TankDriveDistanceAction complete";
			logger.startData("tankdrivedistanceaction_complete")
				.addData("time", getTankDrive().getRobot().getTime() - start_time_)
				.endData();
			logger.endMessage();
		}
	} else {
		getTankDrive().setMotorsToPercents(0.0, 0.0);
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

} // namespace base
} // namespace xero
