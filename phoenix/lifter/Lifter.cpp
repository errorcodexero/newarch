#include "Lifter.h"

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
			encoder_ = std::make_shared<frc::Encoder>(ENCODER_A_CHANNEL,ENCODER_B_CHANNEL);
			brake_solenoid_ = std::make_shared<frc::Solenoid>(BRAKE_SOLENOID_CHANNEL);
			upper_limit_sensor_ = std::make_shared<frc::DigitalInput>(UPPER_LIMIT_SENSOR_CHANNEL);
			lower_limit_sensor_ = std::make_shared<frc::DigitalInput>(LOWER_LIMIT_SENSOR_CHANNEL);
			motors_.push_back(std::make_shared<frc::VictorSP>(0));
			motors_.push_back(std::make_shared<frc::VictorSP>(1));
			gear_solenoid_ = std::make_shared<frc::Solenoid>(1);


		}
		Lifter::~Lifter(){
			
		}
		void Lifter::run() {

		}
		void Lifter::computeState() {

		}
		bool Lifter::canAcceptAction(xero::base::ActionPtr Action) {

		}
	}
}