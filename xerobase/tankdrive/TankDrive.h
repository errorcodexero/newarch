#pragma once

#include <list>

#ifdef SIM
#include <TalonSRX.h>
#else
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>
#endif

#include "Drivebase.h"

typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
	namespace base {
		typedef std::shared_ptr<TalonSRX> TalonPtr;

		/// \brief a tank-style drivebase
		class TankDrive : public Drivebase {
			friend class TankDriveDistanceAction;
			friend class TankDriveVelocityAction;

		public:
			/// \brief Create a new tank drive object
			/// \param robot The robot that contains this tank drive subsystem
			/// \param left_motor_ids A list of TalonSRX ids for the left side of the drivebase, the first
			/// being the master and the rest being followers
			/// \param right_motor_ids A list of TalonSRX ids for the right side of the drivebase, the first
			/// being the master and the rest being followers
			TankDrive(Robot& robot, std::list<int> left_motor_ids, std::list<int> right_motor_ids);

			/// \brief Return the net distance travelled in inches by the left side of the drivebase.
			/// If the robot travels forward and then back by the same distance, the net distance
			/// is zero and zero will be returned.
			/// \returns The distance traveled by the left side in inches
			double getDistL() const {
				return dist_l_ ;
			}

			/// \brief Return the net distance travled in inches by the right side of the drivebase.
			/// If the robot travels forward and then back by the same distance, the net distance
			/// is zero and zero will be returned.
			/// \returns The distance traveled by the right side in inches
			double getDistR() const {
				return dist_r_ ;
			}

			/// \brief Return the average net distanced travelled by the two sides of the drivebase.
			/// \returns The average net distance travelled by the two sides to the drivebase
			double getDist() const {
				return (dist_r_ + dist_l_) / 2.0;
			}

			double getVelocity() const {
				return velocity_;
			}

			/// \brief Invert the output of all of the selected motors
			/// \param left_motor_ids A list of ids of TalonSRXs on the left side to invert
			/// \param right_motor_ids A list of ids of TalonSRXs on the right side to invert
			void invertMotors(std::list<int> left_motor_ids, std::list<int> right_motor_ids);

			/// \brief Compute the current state of the drivebase.
			/// This method generally reads the input sensors associated with the drivebase and
			/// calculates the current state of the drivebase, including the distance traveled,
			/// speed and acceleration of the robot.  This method is generally called once per
			/// robot loop before any robot controller gets a chance to run.
			void computeState();

		private:
			/// \brief Set the motors to output at the given percentages
			/// \param left_percent the percent output for the left motors
			/// \param right_percent the percent output for the right motors
			void setMotorsToPercents(double left_percent, double right_percent);

			std::list<TalonPtr> left_motors_, right_motors_;

			double dist_l_, dist_r_;
			double last_dist_;
			double velocity_;
		};
	}
}
