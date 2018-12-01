#pragma once

#include "Action.h"
#include "basegroups.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		class TankDrive;

		/// \brief This is the base class for all actions for the to be assigned to the TankDrive class.
		/// \sa TankDrive
		class TankDriveAction : public Action {
		public:
			/// \brief Create a new TankDriveAction
			/// \param tank_drive the tank drive subsystem
			TankDriveAction(TankDrive &tank_drive) : tank_drive_(tank_drive) {
			}

		protected:
			TankDrive &getTankDrive() {
				return tank_drive_ ;
			}

		private:
			/// \brief The tank drive subsystem
			TankDrive &tank_drive_;
		};
	}
}
