#pragma once

#include "TubCollectorAction.h" 

namespace xero {
    namespace ranseur {
        class Intake ;

        /// \brief an action that sets the duty cycle of the two intake motors
        class TubCollectorDutyCycleAction : public TubCollectorAction {
        public:
            /// \brief Create a new action object
            /// \param intake the subsystem this action effects
            /// \param duty the duty cycle to set the motors to
            TubCollectorDutyCycleAction(TubCollector &intake, double duty) ;

            /// \brief Create a new action object
            /// \param intake the subsystem this action effects
            /// \param name the name of a paramter that is the value
            TubCollectorDutyCycleAction(TubCollector &intake, const std::string &name) ;

            /// \brief Destroy the action object
            virtual ~TubCollectorDutyCycleAction() ;

            /// \brief Start the duty cycle action.  This method actually sets the
            /// motor duty cycles.
            virtual void start() ;

            /// \brief Run the duty cycle action.  This method does nothing.            
            virtual void run() ;

            /// \brief Canel the current action, stops the motors and returns true
            virtual void cancel() ;

            /// \brief Returns a human readable string for the action
            virtual std::string toString() ;
            
        private:
            double duty_cycle_;
        };
    }
}
