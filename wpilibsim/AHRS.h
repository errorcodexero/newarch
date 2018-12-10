#pragma once

#include "SimulatedObject.h"
#include "SPI.h"
#include <string>

class AHRS : public xero::sim::SimulatedObject
{
public:
	AHRS(const char *port_p);
	AHRS(frc::SPI::Port p) ;
	virtual ~AHRS();

	bool IsConnected()
	{
		return true;
	}

	bool IsCalibrating() {
		return false ;
	}

	double GetYaw();
	void ZeroYaw();

	void SimulatorSetYaw(double yaw) {
		yaw_ = yaw ;
	}

private:
	double yaw_;
};

