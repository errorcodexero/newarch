#pragma once

#include <PIDCtrl.h>
#include <StallMonitor.h>
#include <TrapezoidalProfile.h>

#include "TankDriveAction.h"
#include "TankDrive.h"

#include <list>

/// \file


namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveDistanceAction : public TankDriveAction {
		public:
			/// \brief Create a new TankDriveDistanceAction for the given distance
			/// \param tank_drive the tank drive subsystem
			/// \param target_distance The distance to drive in inches
			TankDriveDistanceAction(TankDrive &tank_drive, double target_distance);

			/// \brief Create a new TankDriveDistanceAction for the given distance
			/// \param tank_drive the tank drive subsystem
			/// \param target_distance The distance to drive in inches
            /// \param maxv the maximum velocity to drive during this action            
			TankDriveDistanceAction(TankDrive &tank_drive, double target_distance, double maxv);			

			/// \brief Create a new TankDriveDistanceAction for the given distance
			/// \param tank_drive the tank drive subsystem
			/// \param distname the name of the parameter that contains the distance to drive     
			TankDriveDistanceAction(TankDrive &tank_drive, const std::string &distname);	

			/// \brief Create a new TankDriveDistanceAction for the given distance
			/// \param tank_drive the tank drive subsystem
			/// \param distname the name of the parameter that contains the distance to drive
            /// \param maxvname the name of the parameter that contains the maximum velocity during this action	
			TankDriveDistanceAction(TankDrive &tank_drive, const std::string &distname, const std::string &maxvname) ;
            
            /// \brief destroy this action
			virtual ~TankDriveDistanceAction() ;

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

            /// \brief add an action that is triggered when the given distance is reached
            /// \param dist the distance when the action is triggered
            /// \param act the action to trigger
			void addTriggeredAction(double dist, ActionPtr act) ;

            /// \brief add an action that is triggred when the given distance is reached
            /// \param distname the name of the parameter that contains the distance when the action
            /// is triggered
            /// \param act the action to trigger
			void addTriggeredAction(const std::string &distname, ActionPtr act) ;

		private:
			void checkTriggeredEvents(double dist) ;

		private:
			xero::misc::PIDCtrl velocity_pid_ ;
			xero::misc::PIDCtrl angle_pid_;
			std::shared_ptr<xero::misc::TrapezoidalProfile> profile_;
			double start_time_ ;
			double profile_start_time_;	
			double profile_initial_dist_;
			double distance_threshold_;
			double target_distance_;
			double profile_outdated_error_long_;
			double profile_outdated_error_short_ ;
			double profile_outdated_error_dist_ ;
			double total_dist_so_far_ ;
			double start_angle_ ;
			bool is_done_;

			std::list<std::pair<double, ActionPtr>> triggered_actions_ ;
			std::list<ActionPtr> running_ ;
		};
	}
}
