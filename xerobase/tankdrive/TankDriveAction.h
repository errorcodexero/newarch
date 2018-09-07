#pragma once

#include "Action.h"
#include "basegroups.h"

namespace xero {
	namespace base {
		class TankDrive;

		/// \brief This is a directive for the drivebase.  All directives for the drivebase should be derived from this class.
		class TankDriveAction : public Action {
		public:
			/// \brief Create a new TankDriveAction
			/// \param tank_drive the tank drive subsystem
			TankDriveAction(TankDrive &tank_drive) : tank_drive_(tank_drive), msg_group_(MSG_GROUP_TANKDRIVE) {}

		protected:
			/// \brief The tank drive subsystem
			TankDrive &tank_drive_;

			/// \brief The tank drive message group
			uint64_t msg_group_;
		};
	}
}
