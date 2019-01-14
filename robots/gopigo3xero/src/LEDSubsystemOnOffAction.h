#pragma once

#include "LEDSubsystemAction.h"
#include "LEDSubsystem.h"

namespace xero
{
    namespace gopigo
    {
        class LEDSubsystemOnOffAction : public LEDSubsystemAction
        {
        public:
            LEDSubsystemOnOffAction(LEDSubsystem &sub, LEDSubsystem::LED led, double r, double g, double b);
            virtual ~LEDSubsystemOnOffAction();

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
            LEDSubsystem::LED led_;
            double r_;
            double g_;
            double b_;
        };
    } // namespace gopigo
} // namespace xero