#pragma once

#include "Action.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		namespace tankdrive {
			/// \brief This is a directive for the drivebase.  All directives for the drivebase should be derived from this class.
			class TankDriveAction : public Action {
			public:
				/// \brief Create a new TankDriveAction
				/// \param parent the parent subsystem
				TankDriveAction(TankDrive &parent) : parent_(parent) {}

			protected:
				TankDrive &parent_;
			};
		}
	}
}
