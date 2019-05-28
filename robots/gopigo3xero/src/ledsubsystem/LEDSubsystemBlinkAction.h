#pragma once

#include "LEDSubsystemAction.h"
#include "LEDSubsystem.h"

namespace xero
{
namespace gopigo3
{
class LEDSubsystem;

class LEDSubsystemBlinkAction : public LEDSubsystemAction
{
    public:
        LEDSubsystemBlinkAction(LEDSubsystem &sub, LEDSubsystem::LED led, double on, double off, double r, double g, double b);
        virtual ~LEDSubsystemBlinkAction();

        /// \brief Start the duty cycle action.  This method actually sets the
        /// motor duty cycles.
        virtual void start();

        /// \brief Run the duty cycle action.  This method does nothing.
        virtual void run();

        /// \brief Signals if this action is done, always returs true
        virtual bool isDone();

        /// \brief Canel the current action, stops the motors and returns true
        virtual void cancel();

        /// \brief Returns a human readable string for the action
        virtual std::string toString();

    private:
        void turnonoff(bool on);

    private:
        LEDSubsystem::LED led_;
        double r_;
        double g_;
        double b_;
        double on_;
        double off_;
        double start_;
        bool state_;
};
} // namespace gopigo3
} // namespace xero