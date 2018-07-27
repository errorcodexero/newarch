#include "TankDrive.h"
#include <cassert>

#ifdef SIM
#include <SensorCollection.h>
#else
#include <ctre/phoenix/MotorControl/SensorCollection.h>
#endif

namespace xero {
	namespace base {

		//////////////////////////////////////////////////////////////////
		//
		// VelocityDirective
		//
		//////////////////////////////////////////////////////////////////

		TankDrive::VelocityDirective::VelocityDirective(double target_velocity) {
			target_velocity_ = target_velocity;
		}

		void TankDrive::VelocityDirective::start() {
		}

		void TankDrive::VelocityDirective::run() {
		}

		void TankDrive::VelocityDirective::end() {
		}

		bool TankDrive::VelocityDirective::cancel() {
			return true;
		}

		bool TankDrive::VelocityDirective::isDone() {
			return true;
		}

		std::string TankDrive::VelocityDirective::toString() {
			return "TankDrive::VelocityDirective " + std::to_string(target_velocity_);
		}

		//////////////////////////////////////////////////////////////////
		//
		// DriveStraightDirective
		//
		//////////////////////////////////////////////////////////////////

		TankDrive::DistanceDirective::DistanceDirective(double target_distance) {
			target_distance_ = target_distance;
		}

		void TankDrive::DistanceDirective::start() {
		}

		void TankDrive::DistanceDirective::run() {
		}

		void TankDrive::DistanceDirective::end() {
		}

		bool TankDrive::DistanceDirective::cancel() {
			return true;
		}

		bool TankDrive::DistanceDirective::isDone() {
			return true;
		}

		std::string TankDrive::DistanceDirective::toString() {
			return "TankDrive::DistanceDirective " + std::to_string(target_distance_);
		}

		//////////////////////////////////////////////////////////////////
		//
		// Drivebase
		//
		//////////////////////////////////////////////////////////////////

		void initTalonList(std::list<int>& ids, std::list<TalonPtr>& talons) {
			//Assuming first id will be master
			for(int id : ids) {
				talons.push_back(std::make_shared<TalonSRX>(id));
				if(talons.size() > 0)
					talons.back()->Follow(*talons.front());
			}
		}

		TankDrive::TankDrive(Robot& robot, std::list<int> left_motor_ids, std::list<int> right_motor_ids) : Drivebase(robot) {
			//The two sides should always have the same number of motors and at least one motor each
			assert((left_motor_ids.size() == right_motor_ids.size()) && (left_motor_ids.size() > 0));

			initTalonList(left_motor_ids, left_motors_);
			initTalonList(right_motor_ids, right_motors_);
		}

		void TankDrive::invertMotors(std::list<int> left_motor_ids, std::list<int> right_motor_ids) {
			for(int id : left_motor_ids) {
				for(TalonPtr talon : left_motors_) {
					if(talon->GetDeviceID() == id)
						talon->SetInverted(true);
				}
			}
			for(int id : right_motor_ids) {
				for(TalonPtr talon : right_motors_) {
					if(talon->GetDeviceID() == id)
						talon->SetInverted(true);
				}
			}
		}

		void TankDrive::computeState() {
			Drivebase::computeState();

			dist_l_ = left_motors_.front()->GetSensorCollection().GetQuadraturePosition();
			dist_r_ = right_motors_.front()->GetSensorCollection().GetQuadraturePosition();
		}
	}
}
