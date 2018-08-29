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
                started, //running intake to aquire cube
                grabbing,  //aquiring a cube
                hold  //holding onto a cube

            };

            State state_;

            double grab_time_;

            virtual std::string toString(State state){
                switch(state){
                    case State::reset: return "reset";
                    case State::started: return "started";
                    case State::grabbing: return "grabbing";
                    case State::hold: return "hold";
                    default: return "";
                }
            }
        };
    }
}