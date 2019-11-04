#include <frc/DigitalOutput.h>
#include <frc/RobotBase.h>
#include <GoPiGo3Robot.h>
#include <cassert>

using namespace gopigo3;

namespace frc
{
    DigitalOutput::DigitalOutput(int channel)
    {
        assert(channel < 2);
        channel_ = channel;
        value_ = false;
    }

    DigitalOutput::~DigitalOutput()
    {
    }

    void DigitalOutput::Set(bool v)
    {
        value_ = v;
        GoPiGo3Robot &hw = RobotBase::getRobotHardware();
        int led = channel_ ? GoPiGo3Robot::LED_BLINKER_RIGHT : GoPiGo3Robot::LED_BLINKER_LEFT;
        if (v)
            hw.setLed(led, 0xff, 0, 0);
        else
            hw.setLed(led, 0x00, 0, 0);
    }
}