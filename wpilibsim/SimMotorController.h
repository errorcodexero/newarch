#pragma once

#include "SpeedController.h"

namespace frc
{
	class SimMotorController : public frc::SpeedController
	{
	public:
		SimMotorController(int index);
		virtual ~SimMotorController();

		virtual void Set(double v);

	private:
		int m_index;
	};
}
