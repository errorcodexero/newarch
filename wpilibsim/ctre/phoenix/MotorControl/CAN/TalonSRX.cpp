#include "TalonSRX.h"

namespace ctre {
namespace phoenix {
namespace motorcontrol {
namespace can {
	TalonSRX::TalonSRX(int deviceNumber) {

	}

	void TalonSRX::Follow(TalonSRX& masterToFollow) {

	}

	SensorCollection& TalonSRX::GetSensorCollection() {
		SensorCollection sensor_collection;
		return sensor_collection;
	}
}
}
}
}
