#pragma once

#include <list>

#ifdef SIM
#include <TalonSRX.h>
#else
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>
#endif

#include <Encoder.h>
#include <AHRS.h>
#include <Kinematics.h>
#include <Subsystem.h>

typedef ctre::phoenix::motorcontrol::can::TalonSRX TalonSRX;

namespace xero {
	namespace base {
		typedef std::shared_ptr<TalonSRX> TalonPtr;

		/// \brief a tank-style drivebase
		class TankDrive : public Subsystem {
			friend class TankDriveDistanceAction;
			friend class TankDriveVelocityAction;
			friend class TankDriveCharAction ;

		public:
			/// \brief Create a new tank drive object
			/// \param robot The robot that contains this tank drive subsystem
			/// \param left_motor_ids A list of TalonSRX ids for the left side of the drivebase, the first
			/// being the master and the rest being followers
			/// \param right_motor_ids A list of TalonSRX ids for the right side of the drivebase, the first
			/// being the master and the rest being followers
			TankDrive(Robot& robot, const std::list<int> &left_motor_ids, const std::list<int> &right_motor_ids);

			/// \brief destroy a tank drive object
			virtual ~TankDrive() ;

			/// \brief Create encoders for the tank drive that are not attached to the TalonSRX controllers
			/// \param l1 first digital IO for left encoder
			/// \param l2 second digital IO for left encoder
			/// \param r1 first digital IO for right encoder
			/// \param r2 second digital IO for right encoder						
			void setEncoders(int l1, int l2, int r1, int r2) ;

			/// \brief Return the current angle of the robot relative to its starting angle
			/// \returns The current angle of the robot
			double getAngle() const {
				return angle_ ;
			}			

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

			/// \brief Return the velocity of the drive base
			double getVelocity() const {
				return velocity_;
			}

			/// \brief Invert the output of the left motors
			void invertLeftMotors() ;

			/// \brief Invert the output of the left motors
			void invertRightMotors() ;			

			/// \brief Compute the current state of the drivebase.
			/// This method generally reads the input sensors associated with the drivebase and
			/// calculates the current state of the drivebase, including the distance traveled,
			/// speed and acceleration of the robot.  This method is generally called once per
			/// robot loop before any robot controller gets a chance to run.
			virtual void computeState();

			/// \brief Run the subsystem
			virtual void run() ;


			/// \brief This method returns true if the tankdrive subsystem can execute the given action
			/// \param action the section to test to see if it can be executed
			/// \returns true if the action can be executed, false otherwise
			virtual bool canAcceptAction(xero::base::ActionPtr action) ;

		private:
			/// \brief Set the motors to output at the given percentages
			/// \param left_percent the percent output for the left motors
			/// \param right_percent the percent output for the right motors
			void setMotorsToPercents(double left_percent, double right_percent);

			static void initTalonList(const std::list<int>& ids, std::list<TalonPtr>& talons) ;

		private:
			std::list<TalonPtr> left_motors_, right_motors_;

			double dist_l_, dist_r_;
			double last_dist_l_, last_dist_r_ ;
			double last_dist_;
			double velocity_;
			double angle_ ;

			double inches_per_tick_ ;

			std::shared_ptr<frc::Encoder> left_enc_ ;
			std::shared_ptr<frc::Encoder> right_enc_ ;

			AHRS *navx_ ;

			xero::misc::Kinematics *kin_ ;
		};
	}
}
