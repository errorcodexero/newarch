#pragma once

#include "Action.h"

namespace xero {
	namespace base {
		class TankDrive;

		/// \brief This is a directive for the drivebase.  All directives for the drivebase should be derived from this class.
		class TankDriveAction : public Action {
		public:
			/// \brief Create a new TankDriveAction
			/// \param parent the parent subsystem
			TankDriveAction(TankDrive &tank_drive) : tank_drive_(tank_drive) {}

		protected:
			TankDrive &tank_drive_;
		};
	}
}
