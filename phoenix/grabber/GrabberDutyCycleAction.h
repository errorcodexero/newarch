#pragma once

#include "GrabberAction.h" 

namespace xero {
    namespace phoenix {
         class GrabberDutyCycleAction : public xero::phoenix::GrabberAction {
            public:
            GrabberDutyCycleAction(Grabber &grabber, double value);
            virtual ~GrabberDutyCycleAction(); 

            /// \brief Start the calibrate action.
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
            double value_;
        };
    }
}