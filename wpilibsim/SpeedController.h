#pragma once

namespace frc
{
	class SpeedController
	{
	public:
		SpeedController();
		virtual ~SpeedController();

		virtual void Set(double value) = 0;
	};
}

