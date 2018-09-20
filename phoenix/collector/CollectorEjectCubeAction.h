#pragma once

#include "CollectorAction.h" 

namespace xero {
    namespace phoenix {
        class Collector;

		/// \brief Action for the collector to eject a cube we are holding
        class CollectorEjectCubeAction : public xero::phoenix::CollectorAction {
        public:
			/// \brief create the eject cube action
			/// \param collector the collector the action is attached to
			/// \param value the duty cycle of the intake motor
			CollectorEjectCubeAction(Collector &collector, double value);

			/// \brief create the eject cube action
			/// \param collector the collector the action is attached to
			/// \param name the name of the parameter with the intake motor duty cycle
			CollectorEjectCubeAction(Collector &collector, const std::string &name);			

			/// \brief destroy the collect cube action
			virtual ~CollectorEjectCubeAction(); 
			
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

        protected:
               
        private:
			// The start time for running the intake out
			double start_time_;

			// The amount of time to run the intake out to eject
			double outrun_time_;

			// If true, the eject is done (time has elapsed)
			bool isdone_;

			// The out action for the intake
			xero::base::ActionPtr intake_out_;

			// The stop action for the intake
			xero::base::ActionPtr intake_off_;
               
        };
    }
}