#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveAngleCharAction : public TankDriveAction {
		public:
			/// \brief Create the angle characterization action
			/// This action rotates the robot by applying opposite power to the two sides of the drivebase.  The
			/// power applied is sequenced from the start value to the stop value with the increment given.  At each
			/// steps of the way information about rotational velocity and acceleration is provided.
			/// \param db the drivebase to rotate
			/// \param duration the duration of rotation at each of the steps along the way
			/// \param start the starting power for the rotation
			/// \param stop the ending power for the rotation
			/// \param incr the increment in power from the start to the end of rotation
			TankDriveAngleCharAction(TankDrive &db, double duration, double start, double stop, double incr) ;

			/// \brief destroy the action
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