#include "Drivebase.h"
#include <SPI.h>

namespace xero {
	namespace base {
		Drivebase::Drivebase(Robot &robot) : Subsystem(robot, "drivebase"), navx_(frc::SPI::Port::kMXP) {
		}

		void Drivebase::computeState() {
			angle_ = navx_.GetYaw();
		}

		bool Drivebase::setDirective(std::shared_ptr<DrivebaseDirective> directive) {
			//
			// Cast the directive to a drivebase directive to be sure it is valid
			//
			std::shared_ptr<DrivebaseDirective> direct_p = std::dynamic_pointer_cast<DrivebaseDirective>(directive) ;
			if (direct_p == nullptr) {
				//
				// This is not a drivebase directive, so we don't assign it
				//
				return false ;
			}

			//
			// The current directive is not done, we return false
			//
			if (!getDirective()->isDone())
				return false ;

			//
			// Call the base class to set the directive
			//
			if (!Subsystem::setDirective(directive))
				return false ;

			//
			// And now start the directive
			//
			getDirective()->start() ;

			return true ;
		}
	}
}
