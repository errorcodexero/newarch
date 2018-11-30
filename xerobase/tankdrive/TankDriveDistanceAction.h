#pragma once

#include <PIDCtrl.h>
#include <StallMonitor.h>
#include <TrapezoidalProfile.h>

#include "TankDriveAction.h"
#include "TankDrive.h"

#include <list>

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveDistanceAction : public TankDriveAction {
		public:
			/// \brief Create a new TankDriveDistanceAction for the given distance
			/// \param tank_drive the tank drive subsystem
			/// \param target_distance The distance to drive in inches
			TankDriveDistanceAction(TankDrive &tank_drive, double target_distance);
			TankDriveDistanceAction(TankDrive &tank_drive, double target_distance, double maxv);			
			TankDriveDistanceAction(TankDrive &tank_drive, const std::string &distname);			
			TankDriveDistanceAction(TankDrive &tank_drive, const std::string &distname, const std::string &maxvname) ;
			virtual ~TankDriveDistanceAction() ;

			void start();
			void run();
			void cancel();
			bool isDone();
			std::string toString();

			void addTriggeredAction(double dist, ActionPtr act) ;
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
