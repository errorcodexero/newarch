#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveAngleCharAction : public TankDriveAction {
		public:
			TankDriveAngleCharAction(TankDrive &subsystem, double duration, double start, double stop, double incr) ;
			virtual ~TankDriveAngleCharAction() ;

			/// \brief Start the action; called once per action when it starts
            virtual void start() ;

			/// \brief Manage the action; called each time through the robot loop
            virtual void run() ;

			/// \brief Cancel the action
            virtual void cancel() ;

			/// \brief Return true if the action is complete
			/// \returns True if the action is complete
            virtual bool isDone() ;

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() ;

		private:
			double start_time_ ;
			double duration_ ;
			bool is_done_ ;
            double start_ ;
			double stop_ ;
			double incr_ ;
			double current_ ;
		} ;
	}
}