#pragma once

#include <GPG3MotorController.h>
#include "../SensorCollection.h"
#include "../ControlMode.h"
#include "../NeutralMode.h"
#include <cassert>

namespace ctre {
namespace phoenix {
namespace motorcontrol {
namespace can {

class TalonSRX : public frc::GPG3MotorController {
public:
	TalonSRX(int id) ;

	void Follow(TalonSRX &talon) {		
	}

	ctre::phoenix::motorcontrol::SensorCollection &GetSensorCollection() {
		return sensors_ ;
	}

	void Set(ctre::phoenix::motorcontrol::ControlMode mode, double value) {
		assert(mode == ControlMode::PercentOutput) ;
		frc::GPG3MotorController::Set(value) ;
	}

	void ConfigVoltageCompSaturation(double voltage, int timeout) {
	}

	void EnableVoltageCompensation(bool enable) {
	}

	void SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode mode) {
	}

private:
	ctre::phoenix::motorcontrol::SensorCollection sensors_ ;
} ;

}
}
}
}
