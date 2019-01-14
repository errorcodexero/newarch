#pragma once

#include <string>

namespace frc
{
    class GenericHID
    {
    public:
        enum class RumbleType
        {
            kLeftRumble,
            kRightRumble,
        };

        enum class HIDType
        {
            kUnknown = -1, 
            kXInputUnknown = 0, 
            kXInputGamepad = 1, 
            kXInputWheel = 2,
            kXInputArcadeStick = 3, 
            kXInputFlightStick = 4, 
            kXInputDancePad = 5, 
            kXInputGuitar = 6,
            kXInputGuitar2 = 7, 
            kXInputDrumKit = 8, 
            kXInputGuitar3 = 11, 
            kXInputArcadePad = 19,
            kHIDJoystick = 20, 
            kHIDGamepad = 21, 
            kHIDDriving = 22, 
            kHIDFlight = 23,
            kHID1stPerson = 24
        };

        enum class JoystickHand
        {
            kLeftHand = 0,
            kRightHand = 1,
        };

    public:
        GenericHID(int port);
        virtual ~GenericHID();

        virtual double GetX(JoystickHand hand = JoystickHand::kRightHand) = 0;
        virtual double GetY(JoystickHand hand = JoystickHand::kLeftHand) = 0;

        bool GetRawButton(int button) const;
        bool GetRawButtonPressed(int button) const;
        bool GetRawButtonReleased(int button) const;
        double GetRawAxis(int axis) const;
        int GetPOV(int pov = 0) const;
        int GetAxisCount() const;
        int GetPOVCount() const;
        int GetButtonCount() const;
        HIDType GetType() const;
        std::string GetName() const;
        int GetAxisType(int axis) const;
        int GetPort() const;
        int SetOutput(int outputNumber, bool value) const;
        int SetOutputs(int value) const;
        void SetRumble(RumbleType type, double value);

    private:
        int m_port;
    };
}
