#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"
#include "PIDCtrl.h"
#include "TrapezoidalProfile.h"



namespace xero {
	namespace base {
		class TankDriveAngleAction : public TankDriveAction {
		public:
			TankDriveAngleAction(TankDrive &subsystem, double duration) ;
			TankDriveAngleAction(TankDrive &subsystem, const std::string &name) ;			
			virtual ~TankDriveAngleAction() ;

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
			xero::misc::PIDCtrl velocity_pid_ ;
			std::shared_ptr<xero::misc::TrapezoidalProfile> profile_;
			double start_time_ ;
			double profile_start_time_;	
			double profile_initial_angle_;
			double angle_threshold_;
			double target_angle_;
			double profile_outdated_error_long_;
			double profile_outdated_error_short_ ;
			double profile_outdated_error_angle_ ;
			double total_angle_so_far_ ;
			bool is_done_;
		} ;
	}
}

