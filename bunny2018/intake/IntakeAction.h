#pragma once

#include <Action.h>
#include "Intake.h"

namespace xero {
	namespace bunny2018 {
		/// \brief This is the base class for actions that can be assigned to the shooter
		class IntakeAction : public xero::base::Action {
		public:
			/// \brief Create a new shooter action
			/// \param subsystem Intake subsystem
			IntakeAction(Intake& subsystem) : subsystem_(subsystem) {
			}

		protected:
			Intake& getSubsystem() {
				return subsystem_ ;
			}

		private:
			/// \brief The shooter subsystem
			Intake& subsystem_;
		};
	}
}
