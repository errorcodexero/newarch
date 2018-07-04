#pragma once
class AHRS
{
public:
	AHRS(const char *port_p);
	virtual ~AHRS();

	bool IsConnected()
	{
		return true;
	}

	double GetYaw();
	void ZeroYaw();
};

