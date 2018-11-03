#include "TankDrive.h"
#include "TankDriveAction.h"
#include "Robot.h"
#include <cassert>

#ifdef SIM
#include <SensorCollection.h>
#else
#include <ctre/phoenix/MotorControl/SensorCollection.h>
#endif

using namespace xero::misc ;

namespace xero {
	namespace base {

		TankDrive::TankDrive(Robot& robot, const std::list<int> &left_motor_ids, const std::list<int> &right_motor_ids) : Subsystem(robot, "tankdrive") {
			//The two sides should always have the same number of motors and at least one motor each
			assert((left_motor_ids.size() == right_motor_ids.size()) && (left_motor_ids.size() > 0));

			initTalonList(left_motor_ids, left_motors_);
			initTalonList(right_motor_ids, right_motors_);

			last_dist_ = 0.0;

#ifdef GOPIGO
			navx_ = new AHRS("/dev/ttyACM0") ;	
#else
			navx_ = new AHRS(frc::SPI::Port::kMXP) ;		
#endif			

			if (!navx_->IsConnected()) {
				delete navx_ ;
				navx_ = nullptr ;
				kin_ = new Kinematics(4.6063, 0.95) ;
				angle_ = 0.0 ;
			}
		}

		TankDrive::~TankDrive() {			
		}

		bool TankDrive::canAcceptAction(ActionPtr ptr) {
			auto act = std::dynamic_pointer_cast<TankDriveAction>(ptr) ;
			return act != nullptr ;
		}

		void TankDrive::run() {
			Subsystem::run() ;
		}

		void TankDrive::setEncoders(int l1, int l2, int r1, int r2) {
			left_enc_ = std::make_shared<frc::Encoder>(l1, l2) ;
			right_enc_ = std::make_shared<frc::Encoder>(r1, r2) ;

			inches_per_tick_ = getRobot().getSettingsParser().getDouble("tankdrive:inches_per_tick") ;

			left_enc_->Reset() ;
			right_enc_->Reset() ;
  		}

		void TankDrive::setGearShifter(int index) {
			gear_ = std::make_shared<frc::Solenoid>(index) ;
		}

		void TankDrive::initTalonList(const std::list<int>& ids, std::list<TalonPtr>& talons) {
			//Assuming first id will be master
			for(int id : ids) {
				talons.push_back(std::make_shared<TalonSRX>(id));
				if(talons.size() > 1)
					talons.back()->Follow(*talons.front());
			}
		}		

		void TankDrive::invertLeftMotors() {
			for(TalonPtr talon : left_motors_)
				talon->SetInverted(true);
		}

		void TankDrive::invertRightMotors() {
			for(TalonPtr talon : right_motors_)
				talon->SetInverted(true);
		}		

		void TankDrive::computeState() {

			if (left_enc_ != nullptr) {
				assert(right_enc_ != nullptr) ;

				dist_l_ = left_enc_->Get() * inches_per_tick_ ;
				dist_r_ = right_enc_->Get() * inches_per_tick_ ;
			}
			else {
				dist_l_ = left_motors_.front()->GetSensorCollection().GetQuadraturePosition();
				dist_r_ = right_motors_.front()->GetSensorCollection().GetQuadraturePosition();
			}

			if (navx_ != nullptr) {
				angle_ = -navx_->GetYaw();
			}
			else {
				kin_->move(dist_l_ - last_dist_l_, dist_r_ - last_dist_r_) ;
				angle_ = kin_->getAngle() ;
			}

			double current_dist = getDist();
			velocity_ = (current_dist - last_dist_) / getRobot().getDeltaTime();
			last_dist_ = current_dist;		

			angular_velocity_ = (angle_ - last_angle_) /getRobot().getDeltaTime();
			last_angle_ = angle_;

			acceleration_ = (velocity_ - last_velocity_) /getRobot().getDeltaTime();
			last_velocity_ = velocity_;

			angular_acceleration_ = (angular_velocity_ - last_angular_velocity_) /getRobot().getDeltaTime();
			last_angular_velocity_ = angular_velocity_;

			last_dist_l_ = dist_l_ ;
			last_dist_r_ = dist_r_ ;	
		}

		void TankDrive::setMotorsToPercents(double left_percent, double right_percent) {
			left_motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, left_percent);
			right_motors_.front()->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, right_percent);
		}
	}
}
