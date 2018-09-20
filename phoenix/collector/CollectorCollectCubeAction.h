#pragma once

#include "CollectorAction.h" 

namespace xero {
    namespace phoenix {
		/// \brief This action when assigned to the collector, collects a cube.
		/// The action starts by opening the grabber and running the intake in. 
		/// When a cube is detected by the collector, the grabber arms close on the
		/// cube and the intake is stopeed.
        class CollectorCollectCubeAction : public xero::phoenix::CollectorAction {
        public:
			/// \brief create the collect cube action
			/// \param collector the collector for the action
            CollectorCollectCubeAction(Collector &collector);

			/// \brief destroy the collect cube action
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
			// These are the internal states for the collector
            enum class State {
                reset, 					// not doing anything
                wait_for_cube,			// grabber open, intake running in, waiting for cube
                secure_cube, 			// closing the grabber arms while running intake in
                hold  					// holding onto a cube with grabber with intake off
            };

        private:
			// Return a human readable string for a given state
            std::string stateToString(State state);

        private:

			// THe current collector state
            State state_;

			// The amount of time to remain in the secure_cube state
            double grab_time_;

			// The actual time we entered the secure_cube state
            double start_grab_;

			// Action for grabber to open grabber to collect angle
            xero::base::ActionPtr grabber_open_;

			// Action for intake to run intake in to collect cube
            xero::base::ActionPtr intake_in_;

			// Action for grabber to hold onto cube
            xero::base::ActionPtr hold_cube_;

			// Action for intake to turn intake off
            xero::base::ActionPtr intake_off_;
        };
    }
}