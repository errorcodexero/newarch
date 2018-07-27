#pragma once

#include "SimulatedObject.h"
#include "SPI.h"
#include <string>

class AHRS : public xero::sim::SimulatedObject
{
public:
	AHRS(frc::SPI::Port port);
	virtual ~AHRS();

	bool IsConnected()
	{
		return true;
	}

	double GetYaw();
	double GetAngle() ;
	void ZeroYaw();

	void SimulatorSetYaw(double yaw) {
		yaw_ = yaw ;
	}

	void SimulatorSetAngle(double ang) {
		angle_ = ang ;
	}

private:
	std::string port_ ;
	double yaw_;
	double angle_ ;
};

