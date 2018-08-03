#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"

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
			double target_velocity_;
		};
	}
}
