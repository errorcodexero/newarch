#pragma once

#include "CollectorAction.h" 

namespace xero {
    namespace phoenix {
         class CollectorCollectCubeAction : public xero::phoenix::CollectorAction {
            public:
            CollectorCollectCubeAction(Collector &Collector);
            virtual ~CollectorCollectCubeAction(); 

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
            enum class State {
                reset, //not doing anything
                wait_for_cube,
                secure_cube, //after collecting cube, wait to make sure it's secure 
                hold  //holding onto a cube

            };

        private:
            std::string stateToString(State state);

        private:

            State state_;

            double grab_time_;
            double start_grab_;

            xero::base::ActionPtr grabber_open_;
            xero::base::ActionPtr intake_in_;
            xero::base::ActionPtr hold_cube_;
            xero::base::ActionPtr intake_off_;

        };
    }
}