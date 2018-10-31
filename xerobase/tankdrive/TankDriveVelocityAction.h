#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"
#include <PIDCtrl.h>
#include <StallMonitor.h>

namespace xero {
	namespace base {
		/// \brief Drives the drivebase at the given velocity
		class TankDriveVelocityAction : public TankDriveAction {
		public:
			/// \brief Create a new TankDriveVelocityAction for the given velocity
			/// \param tank_drive the tank drive subsystem
			/// \param target_velocity The velocity to drive at in inches / second
			TankDriveVelocityAction(TankDrive &tank_drive, double target_velocity);
			void start();
			void run();
			void cancel();
			bool isDone();
			std::string toString();

		private:
			xero::misc::PIDCtrl velocity_pid_, angle_pid_;
			xero::misc::StallMonitor stall_monitor_;

			bool has_stalled_;


			double start_time_ ;
			double velocity_threshold_;
			double initial_velocity_;
			double target_velocity_;
			double actual_velocity_;
			bool is_done_;
		};
	}
}