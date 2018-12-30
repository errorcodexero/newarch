#include "Joystick.h"

namespace frc
{
	Joystick::Joystick(int port) : GenericHID(port)
	{
	}

	Joystick::~Joystick()
	{
	}

	Joystick *Joystick::GetStickForPort(int port)
	{
		return nullptr;
	}

	void Joystick::SetXChannel(int channel)
	{
	}

	void Joystick::SetYChannel(int channel)
	{
	}

	void Joystick::SetZChannel(int channel)
	{
	}

	void Joystick::SetTwistChannel(int channel)
	{
	}

	void Joystick::Joystick::SetThrottleChannel(int channel)
	{
	}

	void SetAxisChannel(Joystick::AxisType type, int channel)
	{
	}

	int Joystick::GetXChannel() const
	{
		return 0;
	}
	int Joystick::GetYChannel() const
	{
		return 0;
	}

	int Joystick::GetZChannel() const
	{
		return 0;
	}

	int Joystick::GetTwistChannel() const
	{
		return 0;
	}

	int Joystick::GetThrottleChannel() const
	{
		return 0;
	}

	int Joystick::GetAxisChannel(AxisType axis) const
	{
		return 0;
	}

	double Joystick::GetX(JoystickHand hand)
	{
		return 0;
	}

	double Joystick::GetY(JoystickHand hand)
	{
		return 0;
	}

	double Joystick::GetX() const
	{
		return 0;
	}

	double Joystick::GetTwist() const
	{
		return 0;
	}

	double Joystick::GetThrottle() const
	{
		return 0;
	}

	double Joystick::GetAxis(AxisType type) const
	{
		return 0;
	}

	bool Joystick::GetTrigger() const
	{
		return 0;
	}

	bool Joystick::GetTriggerPressed()
	{
		return 0;
	}

	bool Joystick::GetTriggerReleased()
	{
		return 0;
	}

	bool Joystick::GetTop() const
	{
		return 0;
	}

	bool Joystick::GetTopPressed()
	{
		return 0;
	}

	bool Joystick::GetTopReleased()
	{
		return 0;
	}
	
	bool Joystick::GetButton(ButtonType type)
	{
		return 0;
	}

	double Joystick::GetMagnitude() const
	{
		return 0;
	}

	double Joystick::GetDirectionRadians() const
	{
		return 0;
	}

	double Joystick::GetDirectionDegrees() const
	{
		return 0;
	}
}
