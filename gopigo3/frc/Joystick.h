#pragma once

#include "GenericHID.h"

namespace frc
{
    class Joystick : public GenericHID
    {
    public:
        enum class AxisType
        {
            kXAxis,
            kYAxis,
            kZAxis,
            kTwistAxis,
            kThrottleAxis
        };

        enum class ButtonType
        {
            kTriggerButton,
            kTopButton,
        };

    public:
        Joystick(int port);
        virtual ~Joystick();

        static Joystick *GetStickForPort(int port);

        void SetXChannel(int channel);
        void SetYChannel(int channel);
        void SetZChannel(int channel);
        void SetTwistChannel(int channel);
        void SetThrottleChannel(int channel);
        void SetAxisChannel(AxisType type, int channel);
        int GetXChannel() const;
        int GetYChannel() const;
        int GetZChannel() const;
        int GetTwistChannel() const;
        int GetThrottleChannel() const;
        int GetAxisChannel(AxisType axis) const;
        double GetX(JoystickHand hand = JoystickHand::kRightHand) ;
        double GetY(JoystickHand hand = JoystickHand::kRightHand) ;
        double GetX() const;
        double GetTwist() const;
        double GetThrottle() const;
        double GetAxis(AxisType type) const;
        bool GetTrigger() const ;
        bool GetTriggerPressed();
        bool GetTriggerReleased();
        bool GetTop() const;
        bool GetTopPressed();
        bool GetTopReleased();
        bool GetButton(ButtonType type);
        double GetMagnitude() const;
        double GetDirectionRadians() const;
        double GetDirectionDegrees() const;
    };
}

