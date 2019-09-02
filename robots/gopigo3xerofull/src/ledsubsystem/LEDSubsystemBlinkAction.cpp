#include "LEDSubsystemBlinkAction.h"
#include "LEDSubsystem.h"
#include <Robot.h>
#include <iostream>
#include <cassert>

namespace xero
{
namespace gopigo3
{
LEDSubsystemBlinkAction::LEDSubsystemBlinkAction(LEDSubsystem &sub, LEDSubsystem::LED led, double on, double off, double r, double g, double b) : LEDSubsystemAction(sub)
{
    led_ = led;
    on_ = on;
    off_ = off;
    r_ = r;
    g_ = g;
    b_ = b;
}

LEDSubsystemBlinkAction::~LEDSubsystemBlinkAction()
{
}

void LEDSubsystemBlinkAction::turnonoff(bool on)
{
    double r, g, b;

    if (on) {
        r = r_;
        g = g_;
        b = g_;
    }
    else {
        r = 0.0;
        g = 0.0;
        b = 0.0;
    }

    switch (led_)
    {
    case LEDSubsystem::LED::LeftEye:
        getSubsystem().multi_[1]->setColor(r, g, b);
        break;

    case LEDSubsystem::LED::RightEye:
        getSubsystem().multi_[0]->setColor(r, g, b);
        break;

    case LEDSubsystem::LED::LeftBlinker:
        if (r > 0.1)
            getSubsystem().singles_[0]->Set(true);
        else
            getSubsystem().singles_[0]->Set(false);
        break;

    case LEDSubsystem::LED::RightBlinker:
        if (r > 0.1)
            getSubsystem().singles_[1]->Set(true);
        else
            getSubsystem().singles_[1]->Set(false);
        break;
    }
}

/// \brief Start the duty cycle action.  This method actually sets the
/// motor duty cycles.
void LEDSubsystemBlinkAction::start()
{
    turnonoff(true);
    state_ = true;
    start_ = getSubsystem().getRobot().getTime();
}

/// \brief Run the duty cycle action.  This method does nothing.
void LEDSubsystemBlinkAction::run()
{
    if (state_ && getSubsystem().getRobot().getTime() - start_ > on_) {
        turnonoff(false);
        state_ = false;
        start_ = getSubsystem().getRobot().getTime();
    }
    else if (!state_ && getSubsystem().getRobot().getTime() - start_ > off_) {
        turnonoff(true);
        state_ = true;
        start_ = getSubsystem().getRobot().getTime();
    }
}

/// \brief Signals if this action is done, always returs true
bool LEDSubsystemBlinkAction::isDone()
{
    return true;
}

/// \brief Canel the current action, stops the motors and returns true
void LEDSubsystemBlinkAction::cancel()
{
}

/// \brief Returns a human readable string for the action
std::string LEDSubsystemBlinkAction::toString()
{
    std::string ret = "LEDSubsystemBlinkAction";
    ret += " led " + LEDSubsystem::ToString(led_);
    ret += " rgb " + std::to_string(r_) + " " + std::to_string(g_) + " " + std::to_string(b_);
    return ret;
}
} // namespace gopigo3
} // namespace xero
