#pragma once

#include "SPI.h"

class AHRS
{
public:
	AHRS(frc::SPI::Port port);
	virtual ~AHRS();

	bool IsConnected()
	{
		return true;
	}

	double GetYaw();
	void ZeroYaw();
};

