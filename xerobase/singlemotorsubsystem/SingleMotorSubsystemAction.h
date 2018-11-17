#pragma once

#include "Action.h"
#include "basegroups.h"

namespace xero {
	namespace base {
		class SingleMotorSubsystem;

		/// \brief This is a directive for the single motor subsystem.
		class SingleMotorSubsystemAction : public Action {
		public:
			/// \brief Create a new SingleMotorAction
			/// \param subsystem SingleMotor subsystem
			SingleMotorSubsystemAction(SingleMotorSubsystem &subsystem) : subsystem_(subsystem) {
			}

		protected:
			SingleMotorSubsystem &getSubsystem() {
				return subsystem_ ;
			}

		private:
			/// \brief The tank drive subsystem
			SingleMotorSubsystem &subsystem_;
		};
	}
}
