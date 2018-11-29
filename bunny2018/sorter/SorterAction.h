#pragma once

#include <Action.h>
#include "Sorter.h"

namespace xero {
	namespace bunny2018 {
		/// \brief This is a directive for the single motor subsystem.
		class SorterAction : public xero::base::Action {
		public:
			/// \brief Create a new SingleMotorAction
			/// \param subsystem SingleMotor subsystem
			SorterAction(Sorter &subsystem) : subsystem_(subsystem) {
			}

		protected:
			Sorter &getSubsystem() {
				return subsystem_ ;
			}

		private:
			/// \brief The tank drive subsystem
			Sorter &subsystem_;
		};
	}
}
