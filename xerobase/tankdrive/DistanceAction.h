#pragma once

#include "TankDriveAction.h"

namespace xero {
	namespace base {
		namespace tankdrive {
			/// \brief Drives the drivebase straight for a given distance
			class DistanceAction : public TankDriveAction {
			public:
				/// \brief Create a new DistanceAction for the given distance
				/// \param parent the parent subsystem
				/// \param target_distance The distance to drive in inches
				DistanceAction(TankDrive &parent, double target_distance);

				void start();
				void run();
				void cancel();
				bool isDone();
				std::string toString();

			private:
				double target_distance_;
			};
		}
	}
}
