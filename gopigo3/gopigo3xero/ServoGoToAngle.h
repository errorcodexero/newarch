#pragma once

#include "ServoAction.h"

namespace xero {
	namespace gopigo {
		class ServoSubsystem ;

		class ServoGoToAngle : public ServoAction {
		public:
			ServoGoToAngle(ServoSubsystem &sub, double angle) ;
			virtual ~ServoGoToAngle() ;

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
			ServoSubsystem &subsystem_ ;
			double angle_ ;
		} ;
	}
}