#include "TankDriveVelocityAction.h"
#include <math.h>
#include <SettingsParser.h>
#include "Robot.h"
#include "basegroups.h"

using namespace xero::misc;


namespace xero {
namespace base {
TankDriveVelocityAction::TankDriveVelocityAction(TankDrive &tank_drive, double target_velocity) : TankDriveAction(tank_drive) {
	target_velocity_ = target_velocity;
	is_done_ = false;
}

void TankDriveVelocityAction::start() {
	initial_velocity_ = getTankDrive().getVelocity();

	xero::misc::SettingsParser parser = getTankDrive().getRobot().getSettingsParser();

	velocity_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:velocity_pid");
	angle_pid_.initFromSettingsExtended(parser, "tankdrive:distance_action:angle_pid", true);

	stall_monitor_.initFromSettings(parser, "tankdrive:distance_action:stall_monitor");

	velocity_threshold_ = parser.getDouble("tankdrive:distanceaction:velocity_threshold");

	start_time_ = getTankDrive().getRobot().getTime();
}

void TankDriveVelocityAction::run() {
	MessageLogger &logger = getTankDrive().getRobot().getMessageLogger();

	if (!is_done_) {
		if (std::fabs(target_velocity_ - actual_velocity_) > velocity_threshold_) {
			
			double current_velocity = getTankDrive().getVelocity();	
			
			double base_power = velocity_pid_.getOutput(target_velocity_, current_velocity, 0.0, getTankDrive().getRobot().getDeltaTime());

			double current_angle = getTankDrive().getAngle();
			double straightness_offset = angle_pid_.getOutput(0, current_angle, 0, getTankDrive().getRobot().getDeltaTime());
			double left_power = base_power - straightness_offset;
			double right_power = base_power + straightness_offset;

			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
			logger << "TankDriveVelocityAction: time " << getTankDrive().getRobot().getTime() ;
			logger << ", target " << target_velocity_;
			logger << ", actual " << current_velocity ;
			logger << ", left " << left_power << ", right " << right_power ;
			logger.endMessage();			

			getTankDrive().setMotorsToPercents(left_power, right_power);
		} 
		
		else {
			is_done_ = true;

			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE);
			logger << "TankDriveDistanceAction complete";
			logger.startData("tankdrivedistanceaction_complete")
				.addData("time", getTankDrive().getRobot().getTime() - start_time_)
				.endData();
			logger.endMessage();
		}
	}
}

void TankDriveVelocityAction::cancel() {
}

bool TankDriveVelocityAction::isDone() {
	return is_done_;
}

std::string TankDriveVelocityAction::toString() {
	return "TankDriveVelocityAction " + std::to_string(target_velocity_);
}

}
}
