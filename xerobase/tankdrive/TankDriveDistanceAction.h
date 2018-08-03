#pragma once

#include <PIDCtrl.h>

#include "TankDriveAction.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveDistanceAction : public TankDriveAction {
		public:
			/// \brief Create a new TankDriveDistanceAction for the given distance
			/// \param tank_drive the tank drive subsystem
			/// \param target_distance The distance to drive in inches
			TankDriveDistanceAction(TankDrive &tank_drive, double target_distance);

			void start();
			void run();
			void cancel();
			bool isDone();
			std::string toString();

		private:
			double initial_dist_;

			xero::misc::PIDCtrl distance_pid_, angle_pid_;

			double distance_threshold_;

			double target_distance_;
			bool is_done_;
		};
	}
}
