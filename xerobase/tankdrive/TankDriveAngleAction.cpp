#include "TankDriveAngleAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>
#include <cmath> 

using namespace xero::misc;

namespace xero {
	namespace base {
		TankDriveAngleAction::TankDriveAngleAction(TankDrive &tank_drive, double target_angle) : TankDriveAction(tank_drive) {
	target_angle_ = target_angle;	
	is_done_ = false;
	double maxa = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:angle_action:maxa") ;
	double maxd = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:angle_action:maxd") ;
	double maxv = getTankDrive().getRobot().getSettingsParser().getDouble("tankdrive:angle_action:maxv") ;		
	profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;
}

TankDriveAngleAction::~TankDriveAngleAction() {
	
}

void TankDriveAngleAction::start() {
	profile_initial_angle_ = getTankDrive().getAngle();

	xero::misc::SettingsParser &parser = getTankDrive().getRobot().getSettingsParser();

	velocity_pid_.initFromSettingsExtended(parser, "tankdrive:angle_action:velocity_pid");
	
	angle_threshold_ = parser.getDouble("tankdrive:angle_action:angle_threshold");
	profile_outdated_error_long_ = parser.getDouble("tankdrive:angle_action:profile_outdated_error_long");
	profile_outdated_error_short_ = parser.getDouble("tankdrive:angle_action:profile_outdated_error_short");
	profile_outdated_error_angle_ = parser.getDouble("tankdrive:angle_action:profile_outdated_error_angle");

	profile_start_time_ = getTankDrive().getRobot().getTime();
	start_time_ = profile_start_time_ ;

	profile_->update(target_angle_, 0.0, 0.0);
	getTankDrive().navx_->ZeroYaw();

	total_angle_so_far_ = 0.0 ;
}

void TankDriveAngleAction::run() {
	MessageLogger &logger = getTankDrive().getRobot().getMessageLogger();

	if (!is_done_) {
		double current_angle = getTankDrive().getAngle();
		double profile_angle_traveled = current_angle - profile_initial_angle_;
		double profile_remaining_angle = target_angle_ - profile_angle_traveled - total_angle_so_far_ ;
		double total_traveled = total_angle_so_far_ + profile_angle_traveled ;

		if (std::fabs(total_traveled - target_angle_) > angle_threshold_) {
			double profile_delta_time = getTankDrive().getRobot().getTime() - profile_start_time_; 
			double profile_target_angle = profile_->getDistance(profile_delta_time);
			double profile_error = std::fabs(profile_target_angle - profile_angle_traveled);

			double current_velocity = getTankDrive().getVelocity();

			bool redo = false ;

			if (profile_remaining_angle < profile_outdated_error_angle_ && profile_error > profile_outdated_error_short_)
				redo = true ;
			else if (profile_remaining_angle > profile_outdated_error_angle_ && profile_error > profile_outdated_error_long_)
				redo = true ;

			if (redo) {
				total_angle_so_far_ += profile_angle_traveled ;

				profile_start_time_ = getTankDrive().getRobot().getTime() ;
				profile_initial_angle_ = getTankDrive().getAngle() ;
				profile_delta_time = 0.0 ;
				profile_->update(target_angle_ - total_traveled, current_velocity, 0.0);

				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
				logger << "Fell behind velocity profile, updating profile";
				logger.endMessage();
			}

			double target_velocity = profile_->getSpeed(profile_delta_time + getTankDrive().getRobot().getTargetLoopTime()) ;

			double base_power = velocity_pid_.getOutput(target_velocity, current_velocity, getTankDrive().getRobot().getDeltaTime());

			double current_angle = getTankDrive().getAngle();
			
			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
			logger << "time " << getTankDrive().getRobot().getTime() ;
			logger << ", angle " << total_traveled ;
			logger << ", profile " << profile_target_angle ;
			logger << ", target " << target_velocity;
			logger << ", actual " << current_velocity ;
			logger << ", base " << base_power ;
            logger << ", current angle "<< current_angle ;
			logger << ", motor " << base_power ;
			logger.endMessage();			

			getTankDrive().setMotorsToPercents(base_power, -base_power);
		} else {
			is_done_ = true;

			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
			logger << "TankDriveAngleAction complete";
			logger.startData("tankdriveangleaction_complete")
				.addData("time", getTankDrive().getRobot().getTime() - start_time_)
				.endData();
			logger.endMessage();
		}
	} else {
		getTankDrive().setMotorsToPercents(0.0, 0.0);
	}
}

void TankDriveAngleAction::cancel() {
}

bool TankDriveAngleAction::isDone() {
	return is_done_;
}

std::string TankDriveAngleAction::toString() {
	return "TankDriveAngleAction " + std::to_string(target_angle_);
}
		}
	}
