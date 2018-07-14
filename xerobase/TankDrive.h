#pragma once

#include <list>
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>
#include "Drivebase.h"

typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
	namespace base {
		typedef std::shared_ptr<TalonSRX> TalonPtr;

		class TankDrive : public Drivebase {
		public:
			/// \brief create a new tank drive object
			/// \param robot the robot that contains this tank drive subsystem
			/// \param left_motor_ids a list of TalonSRX ids for the left side of the robot
			/// \param right_motor_ids a list of TalonSRX ids for the right side of the robot
			TankDrive(Robot& robot, std::list<int> left_motor_ids, std::list<int> right_motor_ids);

			/// \brief returns the net distance travelled in inches by the left wheels on the robot
			/// If the robot travels forward and then back by the same distance, the net distance
			/// is zero and zero will be returned.
			/// \returns the distance traveled by the left side in inches
			double getDistL() const {
				return dist_l_ ;
			}

			/// \brief returns the net distance travled in inches by the right wheels on the robot
			/// If the robot travels forward and then back by the same distance, the net distance
			/// is zero and zero will be returned.
			/// \returns the distance traveled by the right side in inches
			double getDistR() const {
				return dist_r_ ;
			}

			/// \brief compute the current state of the robot.
			/// This method generally reads the input sensors associated with the drivebase and
			/// calculates the current state of the drivebase including the distance traveled, the
			/// the speed and acceleration of the robot.  This method is generally called once per
			/// robot loop before any robot controller gets a chance to run.
			void computeState();

		private:
			std::list<TalonPtr> left_motors_, right_motors_;

			double dist_l_, dist_r_;
		};
	}
}
