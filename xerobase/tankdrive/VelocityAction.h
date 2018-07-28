#pragma once

#include "TankDriveAction.h"

namespace xero {
	namespace base {
		namespace tankdrive {
			/// \brief Drives the drivebase at the given velocity
			class VelocityAction : public TankDriveAction {
			public:
				/// \brief Create a new VelocityAction for the given velocity
				/// \param parent the parent subsystem
				/// \param target_velocity The velocity to drive at in inches / second
				VelocityAction(TankDrive &parent, double target_velocity);

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
}
