#include "TankDriveDistanceAction.h"
#include "Robot.h"
#include "basegroups.h"
#include <SettingsParser.h>
#include <cmath>

using namespace xero::misc;

namespace xero {
namespace base {

TankDriveDistanceAction::TankDriveDistanceAction(TankDrive &tank_drive, double target_distance) : TankDriveAction(tank_drive) {
	target_distance_ = target_distance;	
	is_done_ = false;
	double maxa = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:distance_action:maxa") ;
	double maxd = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:distance_action:maxd") ;
	double maxv = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:distance_action:maxv") ;		
	profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;
}

TankDriveDistanceAction::~TankDriveDistanceAction() {
	
}

void TankDriveDistanceAction::start() {
	profile_initial_dist_ = getTankDrive().getDist();

	xero::misc::SettingsParser &parser = getTankDrive().getRobot().getSettingsParser();

	velocity_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:velocity_pid");
	angle_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:angle_pid", true);

	distance_threshold_ = parser.getDouble("tankdrive:distance_action:distance_threshold");
	profile_outdated_error_long_ = parser.getDouble("tankdrive:distance_action:profile_outdated_error_long");
	profile_outdated_error_short_ = parser.getDouble("tankdrive:distance_action:profile_outdated_error_short");
	profile_outdated_error_dist_ = parser.getDouble("tankdrive:distance_action:profile_outdated_error_dist");

	profile_start_time_ = getTankDrive().getRobot().getTime();
	start_time_ = profile_start_time_ ;

	profile_->update(target_distance_, 0.0, 0.0);
	getTankDrive().navx_->ZeroYaw();

	total_dist_so_far_ = 0.0 ;
}

void TankDriveDistanceAction::run() {
	MessageLogger &logger = getTankDrive().getRobot().getMessageLogger();

	if (!is_done_) {
		double current_distance = getTankDrive().getDist();
		double profile_distance_traveled = current_distance - profile_initial_dist_;
		double profile_remaining_distance = target_distance_ - profile_distance_traveled - total_dist_so_far_ ;
		double total_traveled = total_dist_so_far_ + profile_distance_traveled ;

		if (std::fabs(total_traveled - target_distance_) > distance_threshold_) {
			double profile_delta_time = getTankDrive().getRobot().getTime() - profile_start_time_; 
			double profile_target_distance = profile_->getDistance(profile_delta_time);
			double profile_error = std::fabs(profile_target_distance - profile_distance_traveled);

			double current_velocity = getTankDrive().getVelocity();

			bool redo = false ;

			if (profile_remaining_distance < profile_outdated_error_dist_ && profile_error > profile_outdated_error_short_)
				redo = true ;
			else if (profile_remaining_distance > profile_outdated_error_dist_ && profile_error > profile_outdated_error_long_)
				redo = true ;

			if (redo) {
				total_dist_so_far_ += profile_distance_traveled ;

				profile_start_time_ = getTankDrive().getRobot().getTime() ;
				profile_initial_dist_ = getTankDrive().getDist() ;
				profile_delta_time = 0.0 ;
				profile_->update(target_distance_ - total_traveled, current_velocity, 0.0);

				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
				logger << "Fell behind velocity profile, updating profile";
				logger.endMessage();
			}

			double target_velocity = profile_->getSpeed(profile_delta_time + getTankDrive().getRobot().getTargetLoopTime()) ;
			double target_accel = 0.0 ;			// TODO: Get from the profile
			double base_power = velocity_pid_.getOutput(target_velocity, current_velocity, target_accel, getTankDrive().getRobot().getDeltaTime());

			double current_angle = getTankDrive().getAngle();
			double straightness_offset = angle_pid_.getOutput(0, current_angle, 0.0, getTankDrive().getRobot().getDeltaTime());
			double left_power = base_power - straightness_offset;
			double right_power = base_power + straightness_offset;

			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
			logger << "time " << getTankDrive().getRobot().getTime() ;
			logger << ", dist " << total_traveled ;
			logger << ", profile " << profile_target_distance ;
			logger << ", target " << target_velocity;
			logger << ", actual " << current_velocity ;
			logger << ", left " << left_power << ", right " << right_power ;
			logger.endMessage();			

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

		logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
		logger << "time " << getTankDrive().getRobot().getTime() ;
		logger << ", dist " << total_traveled ;
		logger << ", profile " << profile_target_distance ;
		logger << ", target " << target_velocity;
		logger << ", actual " << current_velocity ;
		logger << ", left " << left_power << ", right " << right_power ;
		logger.endMessage();			
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
