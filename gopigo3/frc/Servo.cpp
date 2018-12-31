#include "Servo.h"
#include "RobotBase.h"
#include "GoPiGo3Robot.h"
#include <tgmath.h>

namespace frc
{
    Servo::Servo(uint8_t which)
    {
        if (which == 0)
            m_hw_channel = gopigo3::GoPiGo3Robot::SERVO_1;
        else if (which == 1)
            m_hw_channel = gopigo3::GoPiGo3Robot::SERVO_2;
        else
        {
            std::runtime_error err("illegal servo number, only 0 (SERVO_1) and 1 (SERVO_2) are supported");
            throw err;
        }
    }

    Servo::~Servo()
    {
    }

    void Servo::setPosition(float degrees)
    {
        gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();
        float usec;

        if (degrees > 175.0f)
            degrees = 175.0f;
        else if (degrees < 0.0f)
            degrees = 0.0f;

        usec = std::round((1500.0f - (PULSE_WIDTH_RANGE / 2.0f)) + ((PULSE_WIDTH_RANGE / 180.0f) * degrees));
        hw.setServo(m_hw_channel, static_cast<uint16_t>(usec));
    }
}
