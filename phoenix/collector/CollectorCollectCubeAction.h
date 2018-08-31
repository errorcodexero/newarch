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
                open_grabber, //opens up grabber and starts running intake
                wait_for_cube,
                secure_cube, //after collecting cube, wait to make sure it's secure 
                close_grabber, //quickly close grabber on cube
                hold  //holding onto a cube

            };

            State state_;

            double grab_time_;

            virtual std::string stateToString(State state){
                switch(state){
                    case State::reset: return "reset";
                    case State::open_grabber: return "open grabber";
                    case State::wait_for_cube: return "wait for cube";
                    case State::secure_cube: return "secure cube";
                    case State::close_grabber: return "close grabber";
                    case State::hold: return "hold";
                    default: return "";
                }
            }
        };
    }
}