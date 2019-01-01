#include "AnalogOutput.h"
#include "RobotBase.h"
#include "GoPiGo3Robot.h"
#include <cassert>

using namespace gopigo3;

namespace frc
{
    AnalogOutput::AnalogOutput(int channel)
    {
        assert(channel <= 5);
        channel_ = channel;
        voltage_ = 0.0;
    }

    AnalogOutput::~AnalogOutput()
    {

    }

    void AnalogOutput::SetVoltage(double v)
    {
        int led;
        std::vector<uint8_t> values;

        gopigo3::GoPiGo3Robot &hw = RobotBase::getRobotHardware();

        voltage_ = v;
        uint8_t val = static_cast<uint8_t>(v * 255.0);

        if (channel_ >= 0 && channel_ <= 2)
            led = GoPiGo3Robot::LED_EYE_LEFT;
        else if (channel_ >= 3 && channel_ <= 5)
            led = GoPiGo3Robot::LED_EYE_RIGHT;

        values = hw.getLastLedSetting(led);
        switch(channel_ % 3)
        {
        case 0:
            hw.setLed(led, val, values[1], values[2]);
            break;
        case 1:
            hw.setLed(led, values[0], val, values[2]);
            break;
        case 2:
            hw.setLed(led, values[0], values[1], val);
            break;
        }
    }
}