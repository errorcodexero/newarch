#pragma once

#include "IntakeAction.h" 

namespace xero {
    namespace phoenix {
        class Intake ;

        /// \brief an action that sets the duty cycle of the two intake motors
        class IntakeDutyCycleAction : public IntakeAction {
        public:
            /// \brief Create a new action object
            /// \param intake the subsystem this action effects
            /// \param duty the duty cycle to set the motors to
            IntakeDutyCycleAction(Intake &intake, double duty) ;

            /// \brief Create a new action object
            /// \param intake the subsystem this action effects
            /// \param name the name of a paramter that is the value
            IntakeDutyCycleAction(Intake &intake, const std::string &name) ;

            /// \brief Destroy the action object
            virtual ~IntakeDutyCycleAction() ;

            /// \brief Start the duty cycle action.  This method actually sets the
            /// motor duty cycles.
            virtual void start() ;

            /// \brief Run the duty cycle action.  This method does nothing.            
            virtual void run() ;

            /// \brief Signals if this action is done, always returs true    
            virtual bool isDone() ;

            /// \brief Canel the current action, stops the motors and returns true
            virtual void cancel() ;

            /// \brief Returns a human readable string for the action
            virtual std::string toString() ;
            
        private:
            double duty_cycle_;
        };
    }
}