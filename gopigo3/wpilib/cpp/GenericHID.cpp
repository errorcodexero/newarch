#include <frc/GenericHID.h>
#include <frc/DriverStation.h>

namespace frc
{
    GenericHID::GenericHID(int port)
    {
        m_port = port;
    }

    GenericHID::~GenericHID()
    {
    }

    bool GenericHID::GetRawButton(int button) const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickButton(m_port, button - 1);
    }

    bool GenericHID::GetRawButtonPressed(int button) const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickButtonPressed(m_port, button - 1);
    }

    bool GenericHID::GetRawButtonReleased(int button) const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickButtonReleased(m_port, button - 1);
    }

    double GenericHID::GetRawAxis(int axis) const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickAxis(m_port, axis);
    }

    int GenericHID::GetPOV(int pov) const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickPOV(m_port, pov);
    }

    int GenericHID::GetAxisCount() const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickAxisCount(m_port);
    }

    int GenericHID::GetPOVCount() const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickPOVCount(m_port);
    }

    int GenericHID::GetButtonCount() const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetStickButtonCount(m_port);
    }

    GenericHID::HIDType GenericHID::GetType() const
    {
        return HIDType::kHIDJoystick;
    }

    std::string GenericHID::GetName() const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetJoystickName(m_port);
    }

    int GenericHID::GetAxisType(int axis) const
    {
        DriverStation &ds = DriverStation::GetInstance();
        return ds.GetJoystickAxisType(m_port, axis);
    }

    int GenericHID::GetPort() const
    {
        return m_port;
    }

    int GenericHID::SetOutput(int outputNumber, bool value) const
    {
        return 0;
    }

    int GenericHID::SetOutputs(int value) const
    {
        return 0;
    }

    void GenericHID::SetRumble(RumbleType type, double value)
    {
    }
}
