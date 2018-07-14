#pragma once

#include "../SensorCollection.h"

namespace ctre {
namespace phoenix {
namespace motorcontrol {
namespace can {
	class TalonSRX {
		int device_id_;
		bool inverted_;

	public:
		TalonSRX(int deviceNumber);

		int GetDeviceID();
		void Follow(TalonSRX& masterToFollow);
		SensorCollection& GetSensorCollection();
		void SetInverted(bool invert);
	};
}
}
}
}
