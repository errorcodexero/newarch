#include "TalonSRX.h"

namespace ctre {
namespace phoenix {
namespace motorcontrol {
namespace can {
	TalonSRX::TalonSRX(int deviceNumber) {
		device_id_ = deviceNumber;
		inverted_ = false;
	}

	int TalonSRX::GetDeviceID() {
		return device_id_;
	}

	void TalonSRX::Follow(TalonSRX& masterToFollow) {

	}

	SensorCollection& TalonSRX::GetSensorCollection() {
		SensorCollection sensor_collection;
		return sensor_collection;
	}

	void TalonSRX::SetInverted(bool invert) {
		inverted_ = invert;
	}
}
}
}
}
