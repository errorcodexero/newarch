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
			encoder_ = std::make_shared<frc::Encoder>(ENCODER_A_CHANNEL,ENCODER_B_CHANNEL);
			brake_solenoid_ = std::make_shared<frc::Solenoid>(BRAKE_SOLENOID_CHANNEL);
			upper_limit_sensor_ = std::make_shared<frc::DigitalInput>(UPPER_LIMIT_SENSOR_CHANNEL);
			lower_limit_sensor_ = std::make_shared<frc::DigitalInput>(LOWER_LIMIT_SENSOR_CHANNEL);
			motors_.push_back(std::make_shared<frc::VictorSP>(0));
			motors_.push_back(std::make_shared<frc::VictorSP>(1));
			gear_solenoid_ = std::make_shared<frc::Solenoid>(1);
			calibrate_flag_ = false;

		}
		Lifter::~Lifter(){
			
		}
		void Lifter::run() {

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
				effective_value = 0;
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


		bool Lifter::canAcceptAction(xero::base::ActionPtr Action) {

		}

		void Lifter::calibrate() {
			encoder_->Reset();
            encoder_ticks_ = 0;
            height_ = getRobot().getSettingsParser().getDouble("lifter:base");
            previous_height_ = height_;
			calibrate_flag_ = true;
		}

		void Lifter::setBrake(bool value) {
			brake_solenoid_->Set(value);
		}
	}
}