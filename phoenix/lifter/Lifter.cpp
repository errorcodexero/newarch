#include "Lifter.h"
#include <cassert>
#include "Robot.h"

const int ENCODER_A_CHANNEL = 4;
const int ENCODER_B_CHANNEL = 5;
const int BRAKE_SOLENOID_CHANNEL = 3;
const int UPPER_LIMIT_SENSOR_CHANNEL = 8;
const int LOWER_LIMIT_SENSOR_CHANNEL = 9;
const int GEAR_SOLENOID_CHANNEL = 1;
const int LIFTER_MOTOR_LEFT_CHANNEL = 1;
const int LIFTER_MOTOR_RIGHT_CHANNEL = 0;


namespace xero {
	namespace phoenix {
		Lifter::Lifter(xero::base::Robot& robot) : xero::base::Subsystem(robot, "lifter") {
			height_ = getRobot().getSettingsParser().getDouble("lifter:base");
			previous_height_ = height_;
			encoder_ = std::make_shared<frc::Encoder>(getRobot().getSettingsParser().getInteger("hw:lifter:encoder1"), getRobot().getSettingsParser().getInteger("hw:lifter:encoder2")) ;
			brake_solenoid_ = std::make_shared<frc::Solenoid>(getRobot().getSettingsParser().getInteger("hw:lifter:brake")) ;
			upper_limit_sensor_ = std::make_shared<frc::DigitalInput>(getRobot().getSettingsParser().getInteger("hw:lifter:limit:top")) ;
			lower_limit_sensor_ = std::make_shared<frc::DigitalInput>(getRobot().getSettingsParser().getInteger("hw:lifter:limit:bottom")) ;
			motors_.push_back(std::make_shared<frc::VictorSP>(getRobot().getSettingsParser().getInteger("hw:lifter:motor1"))) ;
			motors_.push_back(std::make_shared<frc::VictorSP>(getRobot().getSettingsParser().getInteger("hw:lifter:motor2"))) ;
			gear_solenoid_ = std::make_shared<frc::Solenoid>(getRobot().getSettingsParser().getInteger("hw:lifter:shifter")) ;
			calibrate_flag_ = false;

		}
		Lifter::~Lifter(){
			
		}
		void Lifter::run() {
			Subsystem::run() ;
		}

		double Lifter::getCurrentHeight() {
			assert(calibrate_flag_);
			return height_;
		}

		double Lifter::getCurrentVelocity() {
			assert(calibrate_flag_);
			return velocity_;
		}

		void Lifter::setMotorsDutyCycle(double value) {
			double effective_value = value;
			if ((upper_limit_sensor_->Get() && (value > 0)) || (lower_limit_sensor_->Get() && (value < 0))) {
				// effective_value = 0;
			}
			for (auto motor : motors_) {
				motor->Set(effective_value);
			}
		}

		void Lifter::computeState() {
			if(calibrate_flag_) {
				previous_height_ = height_;
				encoder_ticks_ = encoder_->Get();
				height_ = encoder_ticks_*(getRobot().getSettingsParser().getDouble("lifter:inches_per_tick")) + getRobot().getSettingsParser().getDouble("lifter:base");
				velocity_ = (height_ - previous_height_)/getRobot().getDeltaTime();
			}
		}


		bool Lifter::canAcceptAction(xero::base::ActionPtr action) {
			std::shared_ptr<LifterAction> act_p = std::dynamic_pointer_cast<LifterAction>(action) ;
			if (act_p == nullptr)
				return false ;

			// TODO: reject LifterGoToHeightAction if not calibreated

			return true ;
		}

		void Lifter::calibrate() {
			encoder_->Reset();
            encoder_ticks_ = 0;
            height_ = getRobot().getSettingsParser().getDouble("lifter:base");
            previous_height_ = height_;
			calibrate_flag_ = true;
		}

		void Lifter::setBrake(bool value) {
			auto brake_solenoid_debug_ = brake_solenoid_->Get();
			brake_solenoid_->Set(!value);
			brake_solenoid_debug_ = brake_solenoid_->Get();
		}
	}
}