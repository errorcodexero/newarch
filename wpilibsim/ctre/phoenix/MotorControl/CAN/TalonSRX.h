#pragma once

#include "../SensorCollection.h"

namespace ctre {
namespace phoenix {
namespace motorcontrol {
namespace can {
	class TalonSRX {
	public:
		TalonSRX(int deviceNumber);

		void Follow(TalonSRX& masterToFollow);
		SensorCollection& GetSensorCollection();
	};
}
}
}
}
