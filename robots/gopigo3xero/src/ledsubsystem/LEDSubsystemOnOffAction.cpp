#include "LEDSubsystemOnOffAction.h"
#include "LEDSubsystem.h"
#include <iostream>
#include <cassert>

namespace xero
{
    namespace gopigo3
    {
        LEDSubsystemOnOffAction::LEDSubsystemOnOffAction(LEDSubsystem &sub, LEDSubsystem::LED led, double r, double g, double b) : LEDSubsystemAction(sub)
        {
            led_ = led;
            r_ = r;
            g_ = g;
            b_ = b;
        }

        LEDSubsystemOnOffAction::~LEDSubsystemOnOffAction()
        {
        }

        /// \brief Start the duty cycle action.  This method actually sets the
        /// motor duty cycles.
        void LEDSubsystemOnOffAction::start()
        {
            switch(led_)
            {
            case LEDSubsystem::LED::LeftEye:
                getSubsystem().multi_[1]->setColor(r_, g_, b_);
                break;

            case LEDSubsystem::LED::RightEye:
                getSubsystem().multi_[0]->setColor(r_, g_, b_);
                break;

            case LEDSubsystem::LED::LeftBlinker:
                if (r_ > 0.1)
                    getSubsystem().singles_[0]->Set(true);
                else
                    getSubsystem().singles_[0]->Set(false);
                break;

            case LEDSubsystem::LED::RightBlinker:
                if (r_ > 0.1)
                    getSubsystem().singles_[1]->Set(true);
                else
                    getSubsystem().singles_[1]->Set(false);
                break;
            }

        }

        /// \brief Run the duty cycle action.  This method does nothing.
        void LEDSubsystemOnOffAction::run()
        {
        }

        /// \brief Signals if this action is done, always returs true
        bool LEDSubsystemOnOffAction::isDone()
        {
            return true;
        }

        /// \brief Canel the current action, stops the motors and returns true
        void LEDSubsystemOnOffAction::cancel()
        {
        }

        /// \brief Returns a human readable string for the action
        std::string LEDSubsystemOnOffAction::toString()
        {
            std::string ret = "LEDSubsystemOnOffAction";
            ret += " led " + LEDSubsystem::ToString(led_);
            ret += " rgb " + std::to_string(r_) + " " + std::to_string(g_) + " " + std::to_string(b_);
            return ret;
        }
    } // namespace gopigo3
} // namespace xero
