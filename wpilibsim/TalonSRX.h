#pragma once

#include "SimulatedObject.h"

namespace ctre {
namespace phoenix {
namespace motorcontrol {
namespace can {

	class TalonSRX : public xero::sim::SimulatedObject
	{
		friend xero::sim::RobotSimBase ;

	public:
		TalonSRX(int index);
		virtual ~TalonSRX();

		virtual void Set(double v);
		virtual double Get() {
			return value_ ;
		}

		int GetDeviceID() {
			return index_ ;
		}

	private:
		int index_ ;
		double value_ ;
	};
}
}
}
}
